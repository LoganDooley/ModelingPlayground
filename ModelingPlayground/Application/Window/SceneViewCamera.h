#pragma once
#include <memory>
#include <glm/mat4x4.hpp>
#include "glad/glad.h"

class InputManager;

class SceneViewCamera
{
public:
    SceneViewCamera(const std::shared_ptr<InputManager>& inputManager, glm::uvec2 screenSize, glm::vec3 position = glm::vec3(0), glm::vec3 look = glm::vec3(0, 0, -1), float near = 0.1f, float far = 100.f, float fovy = 1.f);

    void BindFramebuffer() const;
    void SetViewport() const;
    GLuint GetFramebuffer() const;
    void SetScreenSize(glm::uvec2 screenSize);
    glm::uvec2 GetScreenSize() const;
    float GetAspectRatio() const;
    const glm::mat4& GetCameraMatrix() const;

private:
    void UpdateFramebuffer();
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateCameraMatrix();

    void HandleKeyEvent(int key, int action);
    void HandleCursorPosEvent(double xpos, double ypos);
    void HandleMouseButtonEvent(int button, int action);

    glm::uvec2 m_screenSize;
    float m_aspectRatio;
    float m_near;
    float m_far;
    float m_fovy;
    
    glm::vec3 m_position;
    glm::vec3 m_look;
    
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_cameraMatrix;

    GLuint m_framebuffer;
    GLuint m_framebufferTexture;
    GLuint m_framebufferRenderbuffer;

    float m_movementSpeed;
};
