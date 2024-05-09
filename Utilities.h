#pragma once

#define GLFW_INCLUDE_VULKAN

#include <fstream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int MAX_FRAME_DRAWS = 2;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Vertex data representation
struct Vertex
{
	glm::vec3 pos; // vertex position (x, y, z)
	glm::vec3 col; // vertex color (r, g, b)
};

// Indices (locations) of Queue Families (if they exist at all)
struct QueueFamilyIndices
{
	int graphicsFamily = -1;			// Location of Graphics Queue Family
	int presentationFamily = -1;		// Location of Presentation Queue Family 

	// Check if  queue families are valid
	bool isValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapChainDetails
{
	VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties, e.g. image size/extent
	std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats, e.g. RGBA and size of each colour
	std::vector<VkPresentModeKHR> presentationModes;	// How image should be presented to screen
};

struct SwapChainImage
{
	VkImage image;
	VkImageView imageView;
};

static std::vector<char> readFile(const std::string& filename)
{
	// Open stream from given file
	// std::ios::binary tells stream to read file as binary
	// std::ios::ate tells stream to start reading form end of the file
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	// Check if file stream successfully opened
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open a file!");
	}

	// Get current read position and use to resize file buffer
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);

	// Move read position (seek to) the start of the file
	file.seekg(0);

	// Read the file data into the buffer (stream "fileSize" in total)
	file.read(fileBuffer.data(), fileSize);

	// Close stream
	file.close();

	return fileBuffer;
}

static uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
	// Get properties of physical device memory
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((allowedTypes & (1 << i))														// Index of memory type must corresponding bit in allowedTypes
			&& (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)	// Desired property bit flag are part of memory type's property flags 
		{
			// This memory type is valid, so return its index 
			return i;
		}
	}
}

static void CreateBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags bufferProperties, VkBuffer* buffer, VkDeviceMemory* bufferMemory)
{
	// -- CREATE VERTEX BUFFER -- 
   // Info to create a buffer (doesn't include assigning memory)
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize;											// Size of buffer (all the vertex)
	bufferInfo.usage = bufferUsage;											// Multiple types of buffer possible, we want vertex buffer
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;						// Similar to swapchain images, can share vertex buffers

	VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, buffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Vertex Buffer!");
	}

	// -- GET BUFFER MEMORY REQUIREMENTS -- 
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);

	// -- ALLOCATE MEMORY TO BUUFER --
	VkMemoryAllocateInfo memoryAllocInfo = {};
	memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocInfo.allocationSize = memoryRequirements.size;
	memoryAllocInfo.memoryTypeIndex = FindMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits,		// Index of memory type on Physical Device that has required bit flag
		bufferProperties);				// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : Allows placememt of data straight into buffer after mapping (Otherwise would have specify manully)

	// Allocate memory to VkDeviceMemory
	result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, bufferMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vertex Buffer Memory!");
	}

	// Allocate memory to given vertex buffer
	vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}

static void CopyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
	// Command buffer to hold transfer commands
	VkCommandBuffer transferCommandBuffer;

	// Command buffer details
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = transferCommandPool;
	allocInfo.commandBufferCount = 1;

	// Allocate command buffer from pool 
	vkAllocateCommandBuffers(device, &allocInfo, &transferCommandBuffer);

	// Info to begin the command buffer record
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;				// We're only using command buffer once, so set up for one time submit

	// Begin recording transfer commond;
	vkBeginCommandBuffer(transferCommandBuffer, &beginInfo);

	// Region of data to copy from and to
	VkBufferCopy bufferCopyRegion = {};
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.size = bufferSize;

	// Command to copy src buffer to dst buffer
	vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion);

	// End commands
	vkEndCommandBuffer(transferCommandBuffer);

	// Queue submission information
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCommandBuffer;

	// Submit transform command to transfer queue and wait until it finished
	vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transferQueue);

	// Free temporary command buffer back to pool
	vkFreeCommandBuffers(device, transferCommandPool, 1, &transferCommandBuffer);
}