#pragma once
#include <functional>

class GLFWwindow;

using FrameBufferSizeCallbackFunc = void (*)(GLFWwindow*, int, int);
using KeyCallbackFunc             = void (*)(GLFWwindow*, int, int, int, int);
using MouseCallback               = void (*)(GLFWwindow*, double, double);
using ScrollCallback              = void (*)(GLFWwindow*, double, double);

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

class Window
{
public:
    Window();
    Window(float width, float height);
    ~Window();

    GLFWwindow* glfwWindow() const;
    void        setFrameBufferSizeCallback(FrameBufferSizeCallbackFunc cb);
    void        setKeyCallback(KeyCallbackFunc cb);
    void        setMouseCallback(MouseCallback cb);
    void        setScrollCallback(ScrollCallback cb);

public:
    // clang-format off
    float width() const { return m_width; };
    float height() const { return m_height; };
    // clang-format on

private:
    GLFWwindow* m_window;
    float       m_width;
    float       m_height;
};