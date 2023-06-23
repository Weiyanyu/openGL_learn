#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "log.h"

#include <memory>
#include "window.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main()
{
	Window window(800, 600);
	window.setFrameBufferSizeCallback(frameBufferSizeCallback);
	window.setKeyCallback(keyCallback);

	auto* glfwWindow = window.glfwWindow();

	while (!glfwWindowShouldClose(glfwWindow))
	{
		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
	Window windo();

}