#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include <iostream>

GLFWwindow* window;

void initWindow(std::string wName = "Test Window", const int width = 800, const int height = 600)
{
	// Initialize GLFW
	glfwInit();

	// Set GLFW to not work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

int main()
{
	return 0;
}