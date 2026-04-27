# Interactive Data Structure Visualizer - Instructions

## Getting Started

You can download and run this project on your local machine using one of the two methods below.

### Prerequisites
Before running the project, ensure you have the following installed:
* A C++ Compiler (e.g., GCC/MinGW or MSVC)
* [Raylib](https://www.raylib.com/) (Configured for your development environment)

### Method 1: Clone via Git (Recommended)
If you have Git installed, open your terminal or command prompt and run:
```bash
git clone https://github.com/nttien255/Data-Structure-Visualization
cd Data-Structure-Visualization
```

### Method 2: Manual Download (ZIP)
If you don't want to use Git, you can easily download the entire project manually:
1. Click the green "<> Code" button at the top right of this repository.
2. Select "Download ZIP" from the dropdown menu.
3. Extract the downloaded .zip file to a folder on your computer.
4. Open your terminal or command prompt and navigate (cd) into that extracted folder.

---

## How to Build and Run

**Using CMake:**
```bash
cd build
./DataStructureVisualizer
```

---

## Usage Guide

1. Select a Structure: From the Main Menu, click on the Data Structure you want to explore.
2. Initialize: Use the Init button to generate a random structure or input comma-separated values (e.g., 1 2 10, 2 3 20 for graphs) to build a custom starting state.
3. Interact: Use the input boxes and action buttons (Insert, Extract Top, Run BFS, etc.) to trigger algorithms.
4. Playback Controls: 
   * Toggle between Auto Mode and Step Mode.
   * In Step Mode, use the << and >> buttons to manually walk through the algorithm.
   * Adjust the Speed Slider on the bottom right to speed up or slow down the animations.