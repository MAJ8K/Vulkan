# Vulkan
A Vulkan Renderer

So far I have created an interface class that will organize the currentGraphics API [Vulkan] and enable fluent communication

# Interface
## functions 
### display function:
  - Opens a display from the current display API[GLFW]


### play function:[LOOP]
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
