#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{

}

Mesh::Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, std::vector<Vertex>* vertices)
{
	vertexCount = vertices->size();
	physicalDevice = newPhysicalDevice;
	device = newDevice;

	vertexBuffer = CreateVertexBuffer(vertices); 
}

void Mesh::DestroyVertexBuffer()
{
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
}

VkBuffer Mesh::CreateVertexBuffer(std::vector<Vertex>* vertices)
{
	// -- CREATE VERTEX BUFFER -- 
	// Info to create a buffer (doesn't include assigning memory)
	VkBuffer buffer;
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Vertex) * vertexCount;							// Size of buffer (all the vertex)
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;					// Multiple types of buffer possible, we want vertex buffer
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;						// Similar to swapchain images, can share vertex buffers

	VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Vertex Buffer!");
	}

	// -- GET BUFFER MEMORY REQUIREMENTS -- 
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	// -- ALLOCATE MEMORY TO BUUFER --
	VkMemoryAllocateInfo memoryAllocInfo = {};
	memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocInfo.allocationSize = memoryRequirements.size;
	memoryAllocInfo.memoryTypeIndex = FindMemoryTypeIndex(memoryRequirements.memoryTypeBits,		// Index of memory type on Physical Device that has required bit flag
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);				// VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : Allows placememt of data straight into buffer after mapping (Otherwise would have specify manully)

	// Allocate memory to VkDeviceMemory
	result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, &vertexBufferMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vertex Buffer Memory!");
	}

	// Allocate memory to given vertex buffer
	vkBindBufferMemory(device, buffer, vertexBufferMemory, 0);

	// -- MAP MEMORY TO VERTEX BUFFER --
	void* data;																		// 1. Create a pointer to a point in normal memory
	vkMapMemory(device, vertexBufferMemory, 0, bufferInfo.size, 0, &data);			// 2. "Map" the vertex buffer memory to that point
	memcpy(data, vertices->data(), (size_t)bufferInfo.size);						// 3. Copy memory from vertices vector to the point 
	vkUnmapMemory(device, vertexBufferMemory);										// 4. Unmap the vertex buffer memory

	return buffer;
}

uint32_t Mesh::FindMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties)
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
