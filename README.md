# TQTSolver

A QBF compiler that produces a ***Truncated Quantified Tree (TQT)*** representation of a given Quantified Boolean Formula (QBF). The resulting TQT can in turn be used to count the number of winning strategies for the existential player.

# Installation

Running the toolchain requires the following:
- A C and C++ compiler
- External programs:
  * An installed version of the QBF solver DepQBF (https://lonsing.github.io/depqbf/)
  * An installed version of the QBF equivalence checker QSOLE (https://zenodo.org/records/18590551)
- It should be possible to invoke these programs at the shell level with the commands "depqbf", "qsole"
  
# Steps to run TQTSolver

1. Create the build directory: `cmake -S . -B build`
2. Build the project: `cmake --build build`
3. Run the solver from the project root directory: `./build/qbf_solver test/sample1.qdimacs` 
4. Alternatively, run the solver without QSOLE: `./build/qbf_solver test/sample2.qdimacs --no_qsole`

# Understanding the output

For example, running
`./build/qbf_solver test/sample1.qdimacs`

may produce the following output:
```
Exploring from root.
Exploration finished.
Exploration time: 0.3055 s
Count result: 2
Counting time: 0.0000 s
```

In this example, the number of winning strategies for the existential player (the #QBF answer) is 2.

The solver generates the TQT representation in `test/sample1.tqt`. The reported times have the following meanings:
- Exploration time: The time taken to construct the TQT representation.
- Counting time: The time taken to compute the #QBF result using the generated TQT file.

A detailed exploration log is also stored in `test/sample1.explore`. This file is updated periodically with checkpoints, allowing an interrupted exploration to resume from the last available checkpoint rather than restarting from the root.

# TQT File Format

The TQT file describes the nodes and edges of the representation.

```
head -n 6 test/sample1.tqt
NODE 7
DEPTH 3
VALUE 0
MULTIPLIER_EXPONENT 0
EQUIVALENT_NODE 4
STATUS SEEN

tail -n 7 test/sample1.tqt
EDGE 1 2 1 0
EDGE 1 3 1 1
EDGE 2 4 2 0
EDGE 2 5 2 1
EDGE 5 6 3 0
EDGE 5 7 3 1
```
The node record shown above describes node 7, which is at depth 3. It is marked as equivalent to node 4, which was previously encountered (STATUS SEEN). Its MULTIPLIER_EXPONENT is 0, indicating that no additional multiplicity needs to be included for the count from node 4 in this node.

The edge record `EDGE 5 6 3 0` represents an edge from node 5 to node 6, labelled with variable 3 assigned the value 0.

# Reference

The implementation and further technical details are described in the associated paper, "*On Knowledge Compilation Languages for QBFs*", scheduled to appear in the 46th IARCS Annual Conference on Foundations of Software Technology and Theoretical Computer Science (FSTTCS 2026).
