#pragma once
#include <functional>

class GLFWwindow;

using FrameBufferSizeCallbackFunc = void (*)(GLFWwindow*, int, int);
using KeyCallbackFunc             = void (*)(GLFWwindow*, int, int, int, int);

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class Window
{
public:
    Window();
    Window(int with, int height);

    GLFWwindow* glfwWindow() const;
    void        setFrameBufferSizeCallback(FrameBufferSizeCallbackFunc cb);
    void        setKeyCallback(KeyCallbackFunc cb);

private:
    GLFWwindow* m_window;
};