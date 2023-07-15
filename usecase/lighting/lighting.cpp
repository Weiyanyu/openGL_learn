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
    ShaderProgram LightingCubeShader("../../resource/shader/2-lighting/lighting-cube.vs", "../../resource/shader/2-lighting/lighting-cube.fs");
    ShaderProgram LightingCubeGouraudShader("../../resource/shader/2-lighting/lighting-cube-gouraud.vs", "../../resource/shader/2-lighting/lighting-cube-gouraud.fs");

    Texture texture("../../resource/texture/wall.jpg");
    // texture.setWarpType(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, borderColor);
    // texture.setFilterType(GL_LINEAR, GL_NEAREST);
    Texture textureFace("../../resource/texture/awesomeface.png");
    Texture textureContainer2("../../resource/texture/container2.png");
    Texture textureContainer2Specular("../../resource/texture/container2_specular.png");
    {
        Texture textureContainer2SpecularCopy = textureContainer2Specular;
    }

    // Texture textureContainer2SpecularColor("../../resource/texture/lighting_maps_specular_color.png");
    Texture textureMatrix("../../resource/texture/matrix.jpg");

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

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    // clang-format on

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

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    LightingCubeShader.use();
    LightingCubeShader.setInt("material.diffuse", 0);
    LightingCubeShader.setInt("material.specular", 1);
    LightingCubeShader.setInt("material.emission", 2);

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureContainer2.id());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureContainer2Specular.id());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureMatrix.id());
        // specular color
        // glBindTexture(GL_TEXTURE_2D, textureContainer2SpecularColor.id());

        glBindVertexArray(VAO);

        // render light
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

        LightingCubeShader.use();
        LightingCubeShader.setMat4("view", glm::value_ptr(view));
        LightingCubeShader.setMat4("projection", glm::value_ptr(projection));
        glm::mat4 lightCubeModel(1.0f);
        LightingCubeShader.setMat4("model", glm::value_ptr(lightCubeModel));
        LightingCubeShader.setVec3("viewPos", glm::value_ptr(camera.position()));

        // LightingCubeShader.setVec3("material.specular", glm::value_ptr(glm::vec3(0.633f, 0.727811f, 0.633f)));
        LightingCubeShader.setFloat("material.shininess", 32.0f);
        LightingCubeShader.setVec3("dirLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        LightingCubeShader.setVec3("dirLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        LightingCubeShader.setVec3("dirLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        LightingCubeShader.setVec3("dirLight.direction", glm::value_ptr(glm::vec3(0.0f, 0.0f, -2.0f)));

        LightingCubeShader.setVec3("pointLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        LightingCubeShader.setVec3("pointLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        LightingCubeShader.setVec3("pointLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        LightingCubeShader.setVec3("pointLight.position", glm::value_ptr(lightPos));
        LightingCubeShader.setFloat("pointLight.constant", 1.0f);
        LightingCubeShader.setFloat("pointLight.linear", 0.09f);
        LightingCubeShader.setFloat("pointLight.quadratic", 0.032f);

        LightingCubeShader.setVec3("spotLight.ambient", glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        LightingCubeShader.setVec3("spotLight.diffuse", glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
        LightingCubeShader.setVec3("spotLight.specular", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        LightingCubeShader.setVec3("spotLight.position", glm::value_ptr(lightPos));
        LightingCubeShader.setVec3("spotLight.direction", glm::value_ptr(camera.front()));
        LightingCubeShader.setFloat("spotLight.constant", 1.0f);
        LightingCubeShader.setFloat("spotLight.linear", 0.09f);
        LightingCubeShader.setFloat("spotLight.quadratic", 0.032f);
        LightingCubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        LightingCubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        LightingCubeShader.setFloat("matrixLight", 0.5);
        LightingCubeShader.setFloat("matrixMove", glfwGetTime());

        // LightingCubeGouraudShader.use();
        // LightingCubeGouraudShader.setMat4("view", glm::value_ptr(view));
        // LightingCubeGouraudShader.setMat4("projection", glm::value_ptr(projection));
        // glm::mat4 lightCubeModel(1.0f);
        // LightingCubeGouraudShader.setMat4("model", glm::value_ptr(lightCubeModel));
        // LightingCubeGouraudShader.setVec3("objectColor", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        // LightingCubeGouraudShader.setVec3("lightColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        // LightingCubeGouraudShader.setVec3("lightPos", glm::value_ptr(lightPos));
        // LightingCubeGouraudShader.setVec3("viewPos", glm::value_ptr(camera.position()));

        for(int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0);
            model       = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model       = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            LightingCubeShader.setMat4("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}