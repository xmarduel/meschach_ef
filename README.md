# meschach_ef
my **very old** finite element library ("framework"), based on the **meschach library** for the linear algebra.

- 1D -> lots of fe : P1,P2,P3, Hermite, Splines S2,S3,S4,S5  for periodic geometries or not (for splines too!)
- 2D -> P1,P2,P3
- 3D -> P1,P2

with triangles/tetrahedras only.

- C code similar in structure like the meschach library
- Very nice python wrapper (for 2.7 or 3.7: updated recently!) : functions can be defined in python and plugged into the c solvers.
- Driver programs examples for laplacian, burgers, eigenvalues, stokes, navier-stokes etc. in C or python.
- lots of graphics possibilities (1D, 2D: interactiv with xt window), 1D, 2D, 3D: vtk, silo etc.
- Input parameters in JSON format (json schema for the structure od the parameters - see the demos).

Hopefully a framework with which beginners can learn how to solve FE problems.

As Xcode project (10.1) for macOS 10.14 - maybe you have to adapt the project in order to compile it locally.

External libs to compile separately.
