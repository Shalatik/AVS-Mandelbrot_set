/**
 * @file LineMandelCalculator.h
 * @author Simona Ceskova <xcesko00@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 2023-11-04
 */

#ifndef LINEMANDELCALCULATOR_H
#define LINEMANDELCALCULATOR_H

#include <BaseMandelCalculator.h>

class LineMandelCalculator : public BaseMandelCalculator
{
public:
    LineMandelCalculator(unsigned matrixBaseSize, unsigned limit);
    ~LineMandelCalculator();
    int *calculateMandelbrot();

private:
    int *data;

    float *r_real;
    float *i_imag;
    float *pre_calc_real_arr;
    float *pre_calc_imag_arr;
    // @TODO add all internal parameters
};
#endif
