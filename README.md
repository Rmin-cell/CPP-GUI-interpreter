# C++ Mathematical Equation Interpreter & Visualizer

A GUI application built with Qt/C++ that parses complex mathematical equations, generates parse trees, and visualizes graphs with customizable bounds. Ideal for educational purposes and mathematical exploration.

![Demo](screenshots/demo.png) *Show interface with equation, parse tree, and graph side-by-side*

## Features

- **Equation Parsing & Validation**  
  Supports complex equations with operators (`+`, `-`, `*`, `/`, `^`), functions (`sin`, `cos`, `log`, etc.), and variables
- **Interactive Parse Tree Visualization**  
  Auto-generates syntax trees for equations to demonstrate parsing logic
- **Graph Plotting**  
  Renders 2D graphs with adjustable domain (min/max X values)
- **Error Highlighting**  
  Identifies syntax errors and invalid expressions in real-time
- **Variable Support**  
  Allows single-variable equations (e.g., `f(x) = 2x^2 + 3`)
- **Export Capabilities**  
  Save graphs and parse trees as PNG/SVG
- **Cross-Platform**  
  Works on Windows, Linux, and macOS

## Installation

### Requirements
- Qt 6.x (with Qt Charts module)
- C++17 compiler
- CMake 3.15+

### Build Instructions
```bash
git clone https://github.com/Rmin-cell/CPP-GUI-interpreter.git
cd CPP-GUI-interpreter
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/Qt6 ..
cmake --build .
