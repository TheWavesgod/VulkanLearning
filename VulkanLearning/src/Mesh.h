#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include "Utilities.h"

struct Model
{
	glm::mat4 model;
};


class Mesh
{
public:
	Mesh();
	
	Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices);

	void DestroyMeshBuffers();
	
	~Mesh();

private:
	Model model;

	int vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	int indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkPhysicalDevice physicalDevice;
	VkDevice device;

	void CreateVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	void CreateIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);

public:
	inline int GetVertexCount() { return vertexCount; }

	inline VkBuffer GetVertexBuffer() { return vertexBuffer; }

	inline int GetIndexCount() { return indexCount; }

	inline VkBuffer GetIndexBuffer() { return indexBuffer; }

	inline void SetModel(glm::mat4 newModel) { model.model = newModel; }
	inline Model& GetModel() { return model; }
};

