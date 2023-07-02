#include "camera.h"
#include "log.h"

Camera::Camera()
    : m_position(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    m_up = m_worldUp;
    updateCameraVectors();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yam, float pitch)
    : m_position(position)
    , m_worldUp(up)
    , m_yaw(yam)
    , m_pitch(pitch)
{
    m_up = m_worldUp;
    updateCameraVectors();
}

void Camera::processMouseMovement(double xOffset, double yOffset, bool constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if(constrainPitch)
    {
        if(m_pitch > 89.0f)
            m_pitch = 89.0f;
        if(m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(double yOffset)
{
    if(m_fov >= 1.0f && m_fov <= 45.0f)
        m_fov -= yOffset;
    if(m_fov <= 1.0f)
        m_fov = 1.0f;
    if(m_fov >= 45.0f)
        m_fov = 45.0f;
}

void Camera::move(CameraDirection direction, float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if(direction == CameraDirection::FORWARD)
        m_position += cameraSpeed * m_front;
    if(direction == CameraDirection::BACKWARD)
        m_position -= cameraSpeed * m_front;
    if(direction == CameraDirection::LEFT)
        m_position -= glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
    if(direction == CameraDirection::RIGHT)
        m_position += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::vec3 Camera::position() const
{
    return m_position;
}

float Camera::fov() const
{
    return m_fov;
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}