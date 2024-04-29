#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

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

	/**
	 *  Vulkan functions
	 */ 
	// - Create functions
	void CreateInstance();

	// - Support functions
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool CheckDeviceSuitable(VkPhysicalDevice device);

	// - Get Functions
	void GetPhysicalDevice();
};

