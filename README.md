#### Stručně odpovězte na následující otázky: ######


1. Byla "Line" vektorizace časově efektivní? Proč?
==============================================================================
Vektorizace optimalizuje kód, protože transformuje sérii operací nad skalárními daty do operací na prováděných vektorech.
Takže vektorizovaný "line" kód dokáže provést v jednom kroku cyklu více operací, protože to jsou vektorové instrukce a tím zkrátit čas výpočtu.
Díky tomu je "line" časově efektivnější než referenční zadání.
Ale v porovnání s "batch" ne, protože 'batch' pracuje lépe s pamětí díky rozbití cyklu.


2. Jaká byla dosažena výkonnost v Intel Advisoru pro jednotlivé implementace 
(v GFLOPS)?
==============================================================================
REF: Pro výpočet v nejvnitřejší smyčce: 0.671 GFLOPS (celkově 2.18 GFLOPS, GFLOP Count = 16.680)
LINE: Pro výpočet v nejvnitřejší smyčce: 27.361 GFLOPS (celkově 22.92 GFLOPS, GFLOP Count = 34.474)
BATCH: Pro výpočet v nejvnitřejší smyčce: 31.618 GFLOPS (celkově 18.68 GFLOPS, GFLOP Count = 11.346)
Z tohoto lze vyčíst, že nejlepší implementace je "batch" protože v té nejpodstatnější vektorizované smyčce provádí nejvíce operací ze všech tří.


3. Jaká část kódu byla vektorizována v "Line" implementaci? Vyčteme tuto 
informaci i u batch kalkulátoru?
==============================================================================
U "line" i "batch" byla vektorizována nejvnitřnější smyčka ,v které se provádí hlavní výpočet, cyklus kde byla použita pragma.
Ano, tuto informaci vyčteme i u "batch" v intel advisor, nebo z .optrpt souborů.
V "line" byla smyčka vektorizována na řádku 104.
V "batch" byla smyčka vektorizována na řádku 103.


4. Co vyčteme z Roofline modelu pro obě vektorizované implementace?
==============================================================================
Celkově dává roofline model obraz o výkonu algoritmu, nejlépe vyšla imlementace "batch" - To lze vydět grafu podle umístění bodu cyklu (důležitý je výpočetní výkon v GFLOPS pro nejvnitřejší cyklus, popsaný už v otázce 2.) -čím více je nahoře vpravo tím lépe.
A taky na aritmetické intenzitě, která odpovídá počtu provedených floating-point operací v každém cyklu v poměru k velikosti dat se kterými program pracuje.
BATCH: 0.308 FLOP/Byte
LINE: 0.307 FLOP/Byte
REF: 0.167 FLOP/Byte
Z toho lze vidět že "line" i "batch" jsou efektivnější protože lépe pracují s cache pamětí než 'ref'. Zároveň 'batch' má ještě lepší výsledek než 'line'.


