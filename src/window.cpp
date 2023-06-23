#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "log.h"

Window::Window()
    :Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{

}

Window::Window(int with, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(with, height, "learn-openGL", nullptr, nullptr);
	if (m_window == nullptr)
	{
		glfwTerminate();
        GL_LOG_E("failed to init gl window");
        std::abort();
	}

	glfwMakeContextCurrent(m_window);
	gladLoadGL();
}

GLFWwindow* Window::glfwWindow() const
{
    return m_window;
}

void Window::setFrameBufferSizeCallback(FrameBufferSizeCallbackFunc cb)
{
	glfwSetFramebufferSizeCallback(m_window, cb);
}

void Window::setKeyCallback(KeyCallbackFunc cb)
{
    glfwSetKeyCallback(m_window, cb);
}