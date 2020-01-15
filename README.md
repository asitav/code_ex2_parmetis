# Building and Running #

The code_ex2_metis code uses cmake to build all source files. This is intended for an "out of source build", namely used like this:

* To build and run using available ParMETIS module in K:
```
#!bash
module purge
module load ParMetis_4.0.3_openmpi_1.8.6_gcc cmake_3.14.0

mkdir -p build
cd build
cmake -DPARMETIS_PREFIX=$(which parmetis) ..
#or ccmake -DPARMETIS_PREFIX=$(which parmetis) ..; See further instructions using ccmake below.
make -j install
```
In ccmake just `configure (c)` and `generate and exit (g)`. `configure (c)` button may have to be done more than once before `generate and exit (g)` stage pops out. Ensure `With ParMETIS` is turned `ON`. `Build type` is irrelevant (default is `Release`). Default install directory is `../install` and will be automatically generated. To run:

```
#!bash
cd build
mpiexec -np <nprocs> ../install/bin/parmetis_test # or mpiexec -np <nprocs> ./ex_parmetis/parmetis_test
```

* To build and run using locally installed ParMETIS:

```
#!bash
module purge
module load openmpi_2.1.1_gcc_6.2 gcc_6.2.0 cmake_3.14.0

mkdir -p build
cd build
cmake \
  -DPARMETIS_PREFIX=/u/amishra4/software/parmetis-4.0.3/install/bin/parmetis \
  -DCMAKE_CXX_COMPILER=$( which mpicxx ) -DCMAKE_C_COMPILER=$( which mpicc ) \
  -DCMAKE_PREFIX_PATH=/u/amishra4/software/parmetis-4.0.3/install/ \
  ../
make -j install

# This step is necessary to use shared lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/u/amishra4/software/parmetis-4.0.3/install/lib/

mpiexec -np <nprocs> ../install/bin/partmesh # or mpiexec -np <nprocs> ./ex_partmesh/partmesh
../install/bin/partmesh # or ./ex_partmesh/partmesh # for single proc test
```
