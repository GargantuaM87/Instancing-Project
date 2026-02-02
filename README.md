# Instancing Project
A combination of multiple graphic techniques, with instancing being the main focus point. This project features custom frame buffers, post-processing, gamma correction, anti-aliasing, and more.


## Required Software
- Git
- A C/C++ Compiler (I use GCC in this case)
- CMake
- GLFW
- Assimp
### Installing GLFW
For a stable install of GLFW on Ubuntu:
```bash
sudo apt update
sudo apt install libglfw3-dev libglfw3
```
This will install the main library and development headers for GLFW.

For installation on macOS:
```bash
brew install glfw
```
And if you don't have the Homebrew package manager, then get it!

### Installing Assimp
For command-line tools and the header files on Ubuntu:
```bash
sudo apt-get install libassimp-dev assimp-utils
```

For installation on macOS:
```bash
brew install assimp
```

## External Libraries
This project uses:
- GLFW
- GLM
- GLAD
- stb
- Dear ImGui
- Assimp

## Installation Steps
1. Clone this repository, then head inside its directory
```bash
git clone https://github.com/GargantuaM87/OpenGL-Light-Simulation.git
cd OpenGL-Light-Simulation
```
2. Create a ./build directory
```bash
mkdir build
cd build
```
3. Configure the project with CMake
```bash
cmake ..
```

4. Finally, build the project!
```bash
cmake --build .
```
Make sure the project is run on ./main.cpp from either the command line or whatever other method you can utilize
**WARNING:** Everything might not build correctly. So it's important that you link the project to the appropriate path of glfw3 when you install it!

## Screenshots
