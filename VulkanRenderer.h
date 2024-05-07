#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>

#include <iostream>

#include "Utilities.h"

class VulkanRenderer
{
public: 
	VulkanRenderer();

	int init(GLFWwindow* newWindow);
	void draw();
	void CleanUp();

	~VulkanRenderer();

private:
	GLFWwindow* window;

	int currentFrame = 0;

	// Vulkan components
	VkInstance instance;

	struct 
	{
		VkPhysicalDevice PhysicalDevice;
		VkDevice LogicalDevice;
	} mainDevice;	

	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	VkSurfaceKHR surface;	
	VkSwapchainKHR swapchain;

	std::vector<SwapChainImage> swapChainImages;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	// Debug messenger to handle debug callback
	VkDebugUtilsMessengerEXT debugMessenger;

	// - Pipeline
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	// - Pools
	VkCommandPool graphicsCommandPool;

	// - Utility
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	// - Synchronization
	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderFinished;
	std::vector<VkFence> drawFences;

	/**
	 *  Vulkan functions
	 */ 
	// - Create functions
	void CreateInstance();
	void CreateLogicalDevice();
	void CreateDebugMessenger();
	void CreateSurface();
	void CreateSwapChain();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();
	void CreateSynchronization();

	// - Record functions
	void RecordCommands();

	// - Proxy functions 
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);   

	// - Get Functions
	void GetPhysicalDevice();

	// - Support functions
	// -- Check functions 
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	bool CheckDeviceSuitable(VkPhysicalDevice device);
	bool CheckValidationLayerSupport(std::vector<const char*>* checkLayers);
	// -- Getter functions
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails GetSwapChainDetails(VkPhysicalDevice device);
	// -- Choose Functions
	VkSurfaceFormatKHR ChooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR ChooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
	// -- Create Functions
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

public:
	// Debug callback function
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,				// Specifies the severity of the message
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,			// Refers to a VkDebugUtilsMessengerCallbackDataEXT struct containing the details of the message itself
		void* pUserData)													// Contains a pointer that was specified during the setup of the callback and allows you to pass your own data to it.
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			// Message is important enough to show
		}	

		std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};

