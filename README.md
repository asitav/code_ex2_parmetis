# Building and Running #

The code_ex2_metis code uses cmake to build all source files. This is intended for an "out of source build", namely used like this:

```
#!bash
mkdir build
cd build
ccmake -DPARMETIS_PREFIX=/usr/local/ParMETIS_4.0.3-gnu ..
make -j install
```
In ccmake just `configure (c)` and `generate and exit (g)`. `configure (c)` button may have to be done more than once before `generate and exit (g)` stage pops out. Ensure `With ParMETIS` is turned `ON`. `Build type` is irrelevant (default is `Release`). Default install directory is `../install` and will be automatically generated. To run:

```
#!bash
cd build
mpirun -np <number-of-procs> ../install/bin/parmetis_test # or mpirun -np <nprocs> ./ex_parmetis/parmetis_test
```
