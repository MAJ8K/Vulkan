#include "Interface.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
		std::cerr << "validation layer: " << pCallbackData->pMessage
			<< std::endl << std::endl;
	}
	return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createinfo){
	createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createinfo.messageSeverity = 
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ;
	createinfo.messageType = 
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ;
	createinfo.pfnUserCallback = debugCallback;
	createinfo.pUserData = nullptr; //optional
}

void Interface::createInstance(){
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error("validation layers requested, but not available!");

	VkApplicationInfo appinfo{};
	appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.pApplicationName = "VulkanProj";
	appinfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	appinfo.pEngineName = "Sands&Shards";
	appinfo.engineVersion = VK_MAKE_VERSION(1,0,0);
	appinfo.apiVersion - VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appinfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	if (true){
		VkExtensionProperties vKextensions[50];
		vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,vKextensions);
		std::cout << extensionCount << " available VKextensions:\n";
		for (int i = 0;i < extensionCount; i++)
			std::cout << '\t' << vKextensions[i].extensionName << '\n';
		std::cout << extensions.size() << " required extensions:\n";
		for (int i = 0;i < extensions.size(); i++)
			std::cout << '\t' << extensions[i] << '\n';
		std::cout << std::endl;
	}

	if (vkCreateInstance(&createInfo,nullptr,&instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance!");
}

bool Interface::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}


std::vector<const char*> Interface::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void Interface::setupDebugMessenger(){
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createinfo;
	populateDebugMessengerCreateInfo(createinfo);
	
	if (CreateDebugUtilsMessengerEXT(instance, &createinfo, nullptr, &debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to setup debug messenger");
}
