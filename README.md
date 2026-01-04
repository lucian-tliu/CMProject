## Introduction of the source code

```
.
├── README.md
├── requirements.txt
├── ising_sim.ipynb
├── include
│   ├── flip_algorithm.hpp
│   ├── isingspins.hpp
│   └── lattice.hpp
├──src
│   ├── flip_algorithm.cpp
│   ├── isingspins.cpp
│   └── lattice.cpp
└── module
    ├── ising_bindings.cpp
    └── setup.py
```

To balance high-performance computing with flexible configurability, the core numerical computation components of this project (including the Spins class, the Metropolis sampling algorithm, and the Wolff cluster update algorithm) are implemented in C++.

Building upon this foundation, Python (via *ising_sim.ipynb*) is used to handle experimental workflow control, numerical analysis, and visualization.

Below is a description of the contents and functionalities of the main source files:

- *lattice* : Here define a basic 2D Lattice class to store the spin configuration, which only provides basic single/cluster flips methods.
- *isingspins* : This file defines the IsingSpins class, which inherits from the lattice class. It add the property $J$ and methods for calculating mean energy and magnetization.
- *flip_algorithms* : This file implements the single step Metropolis and Wolff flip. To make it more integrated, it also provides a function to do multiple steps of flip.
- *ising_bindings* : This file defines the protocol for exposing the aforementioned classes and functions as Python interfaces.
- *setup.py* : This file invokes pybind11 and the C++ compiler to compile the aforementioned source files into a Python third-party library.
- *requirements.txt* : This file lists the Python third-party libraries required for running the Jupyter notebook.
- *ising_sim.ipynb* : This file is the Jupyter notebook for running the simulation and analyzing the results.

## How to install and use

To run this project, you need to have a working C++ compiler and Python environment with *requirements.txt* installed.

```bash
cd /path/to/CMProject
pip install -r requirements.txt
cd /path/to/CMProject/module
pip install -e .
```

or

```bash
python setup.py build_ext --inplace
```

You are supposed to create a new environment for this project to avoid dependency conflicts or environment pollution.

