#include "SceneViewCamera.h"

#include <iostream>
#include <ObjectArray.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../InputManager.h"

SceneViewCamera::SceneViewCamera(const std::shared_ptr<InputManager>& inputManager, glm::uvec2 screenSize, glm::vec3 position, glm::vec3 look, float near, float far, float fovy):
    m_screenSize(screenSize),
    m_aspectRatio(static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y)),
    m_near(near),
    m_far(far),
    m_fovy(fovy),
    m_position(position),
    m_look(look),
    m_viewMatrix(glm::mat4(1)),
    m_projectionMatrix(glm::mat4(1)),
    m_cameraMatrix(glm::mat4(1)),
    m_movementSpeed(1.f)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
    UpdateFramebuffer();
    
    inputManager->SubscribeToKeyEvents([this](const int key, const int action)
    {
        HandleKeyEvent(key, action);
    });
    
    inputManager->SubscribeToCursorPosEvents([this](const double xpos, const double ypos)
    {
        HandleCursorPosEvent(xpos, ypos);
    });

    inputManager->SubscribeToMouseButtonEvents([this](const int button, const int action)
    {
        HandleMouseButtonEvent(button, action);
    });
}

void SceneViewCamera::BindFramebuffer() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void SceneViewCamera::SetViewport() const
{
    glViewport(0, 0, static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y));
}

GLuint SceneViewCamera::GetFramebuffer() const
{
    return m_framebuffer;
}

void SceneViewCamera::SetScreenSize(glm::uvec2 screenSize)
{
    if (m_screenSize != screenSize)
    {
        m_screenSize = screenSize;
        m_aspectRatio = static_cast<float>(m_screenSize.x) / static_cast<float>(m_screenSize.y);
        UpdateProjectionMatrix();
        UpdateFramebuffer();
    }
}

glm::uvec2 SceneViewCamera::GetScreenSize() const
{
    return m_screenSize;
}

float SceneViewCamera::GetAspectRatio() const
{
    return m_aspectRatio;
}

const glm::mat4& SceneViewCamera::GetCameraMatrix() const
{
    return m_cameraMatrix;
}

void SceneViewCamera::UpdateFramebuffer()
{
    // Delete old framebuffer
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteTextures(1, &m_framebufferTexture);
    glDeleteRenderbuffers(1, &m_framebufferRenderbuffer);

    // Create framebuffer
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // Create framebuffer texture for color
    glGenTextures(1, &m_framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind color texture to framebuffer for the color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

    // Create framebuffer renderbuffer for depth and stencil
    glGenRenderbuffers(1, &m_framebufferRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_framebufferRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_screenSize.x, m_screenSize.y);

    // Bind renderbuffer for the depth and stencil attachments
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_framebufferRenderbuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << '\n';
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneViewCamera::UpdateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_position + m_look, m_position, glm::vec3(0, 1, 0));
    UpdateCameraMatrix();
}

void SceneViewCamera::UpdateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(m_fovy, m_aspectRatio, m_near, m_far);
    UpdateCameraMatrix();
}

void SceneViewCamera::UpdateCameraMatrix()
{
    m_cameraMatrix = m_projectionMatrix * m_viewMatrix;
}

void SceneViewCamera::HandleKeyEvent(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        std::cout<<"Pressed Key: "<<key<<std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        
    }
}

void SceneViewCamera::HandleCursorPosEvent(double xpos, double ypos)
{
}

void SceneViewCamera::HandleMouseButtonEvent(int button, int action)
{
}
