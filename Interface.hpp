#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>

#include <vector>
#include <cstring>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

class Interface
{
public:
	Interface(uint32_t,uint32_t);
	~Interface();

	void run();

private:
	void* window;
	uint32_t extensionCount;
	uint32_t WIDTH;
	uint32_t HEIGHT;
//
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

private: 
	void initVulkan();
	void mainLoop();
	void cleanup();
// 
	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger();
};

Interface::Interface(uint32_t width, uint32_t height):
	WIDTH(width),HEIGHT(height),extensionCount(0)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(
		WIDTH,HEIGHT,"Vulkan Window",nullptr,nullptr);


	initVulkan();
}

void Interface::initVulkan(){
	createInstance();
}

Interface::~Interface()
{
	cleanup();
	glfwDestroyWindow((GLFWwindow*)window);

	glfwTerminate();
}
void Interface::cleanup(){
	vkDestroyInstance(instance,nullptr);
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
}

void Interface::run(){

	std::cout << "running\n";
	while(!glfwWindowShouldClose((GLFWwindow*)window)){
		glfwPollEvents();
	}
	std::cout << "ran\n";

}

#include "vInstance.cpp"