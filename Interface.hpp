#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

class Interface
{
private:
	void* window;
	uint32_t extensionCount;
public:
	Interface();
	~Interface();

	void display(uint16_t, uint16_t);
	void play();
};

Interface::Interface(): 
	extensionCount(0)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	vkEnumerateInstanceExtensionProperties(
		nullptr, &extensionCount, nullptr);

	std::cout << extensionCount 
	<< " extensions supported\n\n";

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;
}

Interface::~Interface()
{
	glfwTerminate();
}

void Interface::display(uint16_t width, uint16_t height){
	window = glfwCreateWindow(
		width,height,"Vulkan Window",nullptr,nullptr);
}
void Interface::play(){
	while(!glfwWindowShouldClose((GLFWwindow*)window)){
		glfwPollEvents();
	}

	glfwDestroyWindow((GLFWwindow*)window);

}