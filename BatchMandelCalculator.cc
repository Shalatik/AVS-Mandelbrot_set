/**
 * @file LineMandelCalculator.cc
 * @author Simona Ceskova <xcesko00@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 2023-11-04
 */
#include <iostream>
// #include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <immintrin.h>
#include <stdlib.h>
#include <malloc.h>

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator(unsigned matrixBaseSize, unsigned limit) : BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
	data = (int *)aligned_alloc(32, height * width * sizeof(int));

	int *pdata = data;
	memset(pdata, 0, sizeof(data));
	
	r_real = (float *)aligned_alloc(64, width * sizeof(float));
	i_imag = (float *)aligned_alloc(64, width * sizeof(float));
	pre_calc_real_arr = (float *)aligned_alloc(64, width * (sizeof(float)));
	pre_calc_imag_arr = (float *)aligned_alloc(64, height * (sizeof(float)));
}

BatchMandelCalculator::~BatchMandelCalculator()
{
	free(data);
	free(r_real);
	free(i_imag);
	free(pre_calc_imag_arr);
	free(pre_calc_real_arr);

	data = NULL;
	r_real = NULL;
	i_imag = NULL;
	pre_calc_real_arr = NULL;
	pre_calc_imag_arr = NULL;

	// @TODO cleanup the memory
}

int *BatchMandelCalculator::calculateMandelbrot()
{
	// @TODO implement Mandelbrot calculation using SIMD
	int *pdata = data;

	float *r_real_array = r_real;
	float *i_imag_array = i_imag;
	memset(r_real_array, 0, sizeof(r_real));
	memset(i_imag_array, 0, sizeof(i_imag));

	float *pre_calc_real = pre_calc_real_arr;
	float *pre_calc_imag = pre_calc_imag_arr;
	memset(pre_calc_real, 0, sizeof(pre_calc_real_arr));
	memset(pre_calc_imag, 0, sizeof(pre_calc_imag_arr));


	const float limit4 = 4.0f;
	const float var2 = 2.0f;

	for (int i = 0; i < height; i++)
	{
		pre_calc_imag[i] = y_start + i * dy;
	}
	for (int i = 0; i < width; i++)
	{
		pre_calc_real[i] = x_start + i * dx;
	}

	int reduction = 64;

	int in_data = 0;
	int conv_in_data = 0;
	int idk = 0;
	int tmp = 0;
	int help_data_array[width];

	float temp_calculation;
	int height_half = height/2;
	int width_batch = width/64; // width/32/32; 

	for (int h_iter = 0; h_iter < height_half; h_iter++)
	{
		
		int *in_data = data + h_iter*width;
		int *conv_in_data = data + (height-h_iter-1)*width;

		std::fill(help_data_array, help_data_array + width, limit);

		
		for (int g = 0, reduction = 64; g < width_batch; g++)
		{
			for (int l_iter = 0, reduction = 64; (l_iter < limit) && (reduction > 0) ; ++l_iter) 
			{
				#pragma omp simd
				for (int j = 0; j < 64; j++)
				{
					int const w_iter = g*64 + j;
					!l_iter ? r_real_array[w_iter] = pre_calc_real[w_iter], i_imag_array[w_iter] = pre_calc_imag[h_iter] : idk++; //  pdata[in_data + j]

					help_data_array[w_iter] = (((r_real_array[w_iter] * r_real_array[w_iter]) + (i_imag_array[w_iter] * i_imag_array[w_iter])) > limit4 && help_data_array[w_iter] == limit) ? (reduction--, l_iter) : help_data_array[w_iter];

					temp_calculation = i_imag_array[w_iter] * i_imag_array[w_iter]; //T i2 = zImag * zImag;	T r2 = zReal * zReal; 
					i_imag_array[w_iter] = var2 * r_real_array[w_iter] * i_imag_array[w_iter] + pre_calc_imag[h_iter]; //zImag = 2.0f * zReal * zImag + imag;
					r_real_array[w_iter] = r_real_array[w_iter] * r_real_array[w_iter] - temp_calculation + pre_calc_real[w_iter]; //zReal = r2 - i2 + real;
				}
			}
		}
		memcpy(in_data, help_data_array, width*sizeof(int));
		memcpy(conv_in_data, help_data_array, width*sizeof(int));
	}
	return data;
}