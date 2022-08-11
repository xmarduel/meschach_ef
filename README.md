# meschach_ef
my **old** finite element library (years 2000), based on the **meschach library** for the linear algebra.

Just updated/cleaned recently in order to make it public in github.

- 1D -> lots of fe : P1,P2,P3, Hermite, Splines S2,S3,S4,S5  for periodic geometries or not (for splines too!)
- 2D -> P1,P2,P3
- 3D -> P1,P2,P3

with triangles/tetrahedras only.

- C code similar in structure to the meschach library
- Very nice python wrapper (3.7: **updated recently!**) : functions can be defined in python and plugged into the c solvers.
- Driver programs examples for laplacian, burgers, eigenvalues, stokes, navier-stokes etc. in C or python.
- lots of graphics possibilities (1D, 2D: interactiv with xt window), 1D, 2D, 3D: vtk, silo etc.
- Input parameters in JSON format (json schema for the structure od the parameters - see the demos).
- **New**!: Lua interpreter linked-in to evaluate functions given as strings in configurations files

Hopefully a framework with which beginners can learn how to solve FE problems.

As Xcode project (10.1) for macOS 10.14 - maybe you have to adapt the project in order to compile it locally.

**New**! Linux makefile (follow the instructions!)

External libs to compile separately. 

After successfull compiling (and settings some environment variables), just type:
> python DRIVER_1D.py

> python DRIVER_2D.py

> python DRIVER_3D.py

> python DRIVER_KURAMOTO_SIVASHINSKY_b.py  (for fun)

or for C drivers
> 1D.exe

> 2D.exe

> 3D.exe

Please see the html doc for more details
