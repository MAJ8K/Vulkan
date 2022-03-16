# Vulkan
A Vulkan Renderer

So far I have created an interface class that organizes the currentGraphics API [Vulkan].

In the future it will allow for real time communication using code.

# Goals
  - ## 3D
  - ## ObjectFile Handling
  - ## Object Bones & Animations
  - ## Physics
  - ## Ray Tracing
  - ## Voxel Handling

# Interface
## functions 
### Interface Construction:
  - Opens a display from the current DisplayAPI[GLFW]
  - Initializes the GraphicsAPI[Vulkan] for runtime use

### Interface Destruction:
  - cleans up the artifacts left in memory before it is destroyed

### Interface Run Time:[LOOP]
  - loops through events and updates displays(1 max)

# Build Command
```bash
$ g++ -fdiagnostics-color=always -g main.cpp -o main -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr
```
```json
"command": "/usr/bin/g++",
"args": [
	"-fdiagnostics-color=always",
	"-g",
	"main.cpp",
	"-o",
	"main",
	"-lglfw",
	"-lvulkan",
	"-ldl",
	"-lpthread",
	"-lX11",
	// "-lXxf86vm",
	"-lXrandr",
	// "-lXi",
],
```
