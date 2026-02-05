# Instancing Project
A combination of multiple graphic techniques. This project features custom frame buffers, post-processing, gamma correction, anti-aliasing, and more.

To explain what this project contains a little more...

- Instancing: Instantiated ~10000 icospheres around the scene by drawing the meshes as instanced geometry, saving calls between the CPU and GPU. The difference between each geometry (icosphere) lies in its randomized model matrix, which is generated on the CPU side and then sent to the GPU as an attribute. 
- Image Processing: Created a frame buffer that initially contains empty color data through an image texture. It is filled with the scene's color data as an off-screen buffer. With this frame buffer, I can do some post-processing with kernels (convolution matrices).
- Anti-Aliasing: I created another frame buffer that is drawn to with Multi-Sampling Anti-Aliasing (MSAA), then I blitted the information from this MSAA frame buffer to an intermediate frame buffer so I could continue doing post-processing.
- Cube Mapping: A space skybox that is centered around the player. I found the method for keeping this skybox centered around the player to be really fascinating. You take the upper-left 3x3 section of a 4x4 view matrix, leaving the translation part just outside this section, therefore only rotating will change how the skybox looks (scaling too, but there's no way for the player to do that).
- Environment Mapping: Gave the instantiated icospheres reflective/refractive properties by sampling from the surrounding cubemap with calculated vectors. Right now, the icospheres should be reflective, so they'll reflect the cubemap. I will probably add an option to switch between a reflective and refractive vector through ImGui in the future. 
- Gamma Correction: A topic that I'm not as informed about as I want to. When gamma correction is on, the scene is darker; the inverse is true as well. There's a power law relationship between normal physical light intensity and perceptual brightness. A non-linear transformation that we usually apply to images that display physical linear colors to transform them into perceptually linear colors is called gamma correction. In this case, I apply a gamma of 2.2 to the texture colors of the intermediate framebuffer after previous lighting calculations.

There is a few more details to mention, but I encourage you to look through the source code and find any faults in the program and criticism to deliver, as I'm just a beginner in CG.

## Preview
![Showcase](project.gif)
![Post-Processing](kernel.gif)
![Gamma Correction](gammaC.gif)

## Resources
For starting OpenGL: https://learnopengl.com/Introduction
A supplementary introduction while learning OpenGL. This focuses on graphics in a broader sense: https://math.hws.edu/eck/cs424/downloads/graphicsbook-linked.pdf

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

### Installing Assimp
For command-line tools and the header files on Ubuntu:
```bash
sudo apt-get install libassimp-dev assimp-utils
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
git clone https://github.com/GargantuaM87/Instancing-Project.git
cd Instancing-Project
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
**WARNING:** Not all components may build correctly. Therefore, it's essential to link the project to the correct path of glfw3 when you install it.
