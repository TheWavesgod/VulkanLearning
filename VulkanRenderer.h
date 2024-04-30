#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Utilities.h"

class VulkanRenderer
{
public: 
	VulkanRenderer();

	int init(GLFWwindow* newWindow);
	void CleanUp();

	~VulkanRenderer();

private:
	GLFWwindow* window;

	// Vulkan components
	VkInstance instance;

	struct 
	{
		VkPhysicalDevice PhysicalDevice;
		VkDevice LogicalDevice;
	} mainDevice;

	VkQueue graphicsQueue;

	/**
	 *  Vulkan functions
	 */ 
	// - Create functions
	void CreateInstance();
	void CreateLogicalDevice();

	// - Get Functions
	void GetPhysicalDevice();

	// - Support functions
	// -- Check functions 
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool CheckDeviceSuitable(VkPhysicalDevice device);
	// -- Getter functions
	QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device);
};

