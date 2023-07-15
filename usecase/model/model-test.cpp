#include "log.h"
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// clang-format on
#include <iostream>
#include <memory>
#include <cmath>

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "model.h"

float  windowW = 800.0f, windowH = 600.0f;
bool   isWireframeMode = false;
float  mixValue        = 0.0f;
float  deltaTime       = 0.0f; // 当前帧与上一帧的时间差
float  lastFrame       = 0.0f; // 上一帧的时间
float  lastX = windowW / 2, lastY = windowH / 2;
bool   firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0f);

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        isWireframeMode = !isWireframeMode;
        if(isWireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixValue += 0.1f;
        if(mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixValue -= 0.1f;
        if(mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(CameraDirection::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(CameraDirection::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(CameraDirection::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(CameraDirection::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    camera.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}

int main()
{

    Window window(windowW, windowH);
    window.setFrameBufferSizeCallback(frameBufferSizeCallback);
    window.setKeyCallback(keyCallback);
    window.setMouseCallback(mouse_callback);
    window.setScrollCallback(scroll_callback);
    auto* glfwWindow = window.glfwWindow();
    glEnable(GL_DEPTH_TEST);

    ShaderProgram LightingShader("../../resource/shader/2-lighting/lighting.vs", "../../resource/shader/2-lighting/lighting.fs");
    ShaderProgram shader("../../resource/shader/3-model/model.vs", "../../resource/shader/3-model/model.fs");

    // clang-format off
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // vao
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vbo
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    Model model("../../resource/model/nanosuit/nanosuit.obj");

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    LightingShader.use();

    while(!glfwWindowShouldClose(glfwWindow))
    {

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(camera.fov()), window.width() / window.height(), 0.1f, 100.0f);

        // specular color
        // glBindTexture(GL_TEXTURE_2D, textureContainer2SpecularColor.id());

        // render light
        glBindVertexArray(VAO);
        LightingShader.use();
        LightingShader.setMat4("view", glm::value_ptr(view));
        LightingShader.setMat4("projection", glm::value_ptr(projection));
        glm::mat4 lightModel(1.0f);
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        LightingShader.setMat4("model", glm::value_ptr(lightModel));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.use();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        glm::mat4 nanosuitModel(1.0f);
        nanosuitModel = glm::translate(nanosuitModel, glm::vec3(0.0f, 0.0f, 0.0f));
        nanosuitModel = glm::scale(nanosuitModel, glm::vec3(0.1f, 0.1f, 0.1f));
        shader.setMat4("model", glm::value_ptr(nanosuitModel));

        shader.setFloat("material1.shininess", 32.0f);
        shader.setVec3("dirLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        shader.setVec3("dirLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        shader.setVec3("dirLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.setVec3("dirLight.direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, -2.0f)));

        shader.setVec3("pointLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        shader.setVec3("pointLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        shader.setVec3("pointLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.setVec3("pointLight.position", glm::value_ptr(lightPos));
        shader.setFloat("pointLight.constant", 1.0f);
        shader.setFloat("pointLight.linear", 0.09f);
        shader.setFloat("pointLight.quadratic", 0.032f);

        shader.setVec3("spotLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        shader.setVec3("spotLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        shader.setVec3("spotLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        shader.setVec3("spotLight.position", glm::value_ptr(camera.position()));
        shader.setVec3("spotLight.direction", glm::value_ptr(camera.front()));
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        model.draw(shader);
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
}