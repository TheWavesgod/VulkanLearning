#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	
	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, std::vector<Vertex>* vertices);

	void DestroyVertexBuffer();
	
	~Mesh();

private:
	int vertexCount;
	VkBuffer vertexBuffer;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkBuffer CreateVertexBuffer(std::vector<Vertex>* vertices);
	uint32_t FindMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties);

public:
	inline int GetVertexCount() { return vertexCount; }

	inline VkBuffer GetVertexBuffer() { return vertexBuffer; }
};

