#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DEFAULT_CAMERA_YAM -90.0f
#define DEFAULT_CAMERA_PITCH 0.0f
#define DEFAULT_CAMERA_MOVEMENT_SPEED 2.5f
#define DEFAULT_CAMERA_MOUSE_SENSITIVITY 0.1f
#define DEFAULY_CAMERA_FOV 45.0f

enum CameraDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    Camera();
    Camera(const glm::vec3& position, const glm::vec3& up, float yam, float pitch);
    void      processMouseMovement(double xOffset, double yOffset, bool constrainPitch = true);
    void      processMouseScroll(double yOffset);
    void      move(CameraDirection direction, float deltaTime);
    glm::mat4 getViewMatrix();
    glm::vec3 position() const;
    float     fov() const;

protected:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

private:
    // camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_worldUp;
    glm::vec3 m_right;
    glm::vec3 m_up;

    // euler Angles
    float m_yaw   = DEFAULT_CAMERA_YAM;
    float m_pitch = DEFAULT_CAMERA_PITCH;

    // camera options
    float m_movementSpeed    = DEFAULT_CAMERA_MOVEMENT_SPEED;
    float m_mouseSensitivity = DEFAULT_CAMERA_MOUSE_SENSITIVITY;
    float m_fov              = DEFAULY_CAMERA_FOV;
};