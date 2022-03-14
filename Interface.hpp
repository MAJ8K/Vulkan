#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <limits>

struct QueueFamilyIndices {
	uint32_t flags;
	uint32_t graphicsFamily;	//0B01
	uint32_t presentFamily;		//0B10

	bool isComplete(){
		return 0B11 == (flags & 0B11);
	}
};

struct SwapChainSupportDetails{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

const int MAX_FRAMES_IN_FLIGHT = 2;
const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

	bool framebufferResized = false;
private:
	void* window;
	uint32_t extensionCount;
	uint32_t WIDTH;
	uint32_t HEIGHT;
//
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;

private: 
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void cleanupSwapChain();
// 
	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugMessenger();
	void pickPhysicalDevice();
	void createLogicalDevice();

	void createSurface();
	void createSwapChain();
	void recreateSwapChain();
	void createImageViews();

	void createGraphicsPipeline();
	void createRenderPass();

	void createFramebuffers();

	void recordCommandBuffer(VkCommandBuffer, uint32_t);
	void createCommandPool();
	void createCommandBuffers();

	void drawFrame();
	void createSyncObjects();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<Interface*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
};

Interface::Interface(uint32_t width, uint32_t height):
	WIDTH(width),HEIGHT(height),extensionCount(0)
{
	initWindow();
	initVulkan();
}
void Interface::initWindow(){
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan Yatta",nullptr,nullptr);
	glfwSetWindowUserPointer((GLFWwindow*)window,this);
	glfwSetFramebufferSizeCallback((GLFWwindow*)window,framebufferResizeCallback);
}
void Interface::initVulkan(){
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createCommandBuffers();
	createSyncObjects();
}

Interface::~Interface()
{
	cleanup();
	glfwDestroyWindow((GLFWwindow*)window);

	glfwTerminate();
}
void Interface::cleanupSwapChain() {
	
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

		vkDestroySwapchainKHR(device, swapChain, nullptr);
}
void Interface::cleanup(){
	cleanupSwapChain();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	vkDestroyCommandPool(device, commandPool, nullptr);
	
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroyInstance(instance,nullptr);
}

void Interface::run(){

	std::cout << "running\n";
	while(!glfwWindowShouldClose((GLFWwindow*)window)){
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(device);
	std::cout << "ran\n";

}

#include "vInstance.cpp"
#include "vDevices.cpp"
#include "vPresentation.cpp"
#include "vGraphicsPipeline.cpp"
#include "vDrawing.cpp"