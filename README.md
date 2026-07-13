# TQTSolver
A QBF compiler which provides a Truncated Quantified Tree (TQT) representation for a given QBF. 
The TQT can inturn be used to count the number of winning strategies in the QBF.

# Installation:

Running the toolchain requires the following:
- A C and C++ compiler
- External programs:
  * An installed version of the DepQBF QBF solver (https://lonsing.github.io/depqbf/)
  * An installed version of the QSOLE QBF Equivalence checker (https://zenodo.org/records/18590551)
  * It should be possible to invoke these programs at the shell level with the commands "depqbf", "qsole"
  
# Steps to run CQBFSolver

- to create the build directory: cmake -S . -B build
- from inside build directory: make
- to run: ./build/qbf_solver test/sample1.qdimacs (or) ./build/qbf_solver test/sample2.qdimacs --no_qsole
