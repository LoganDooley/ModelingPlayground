#include "SceneViewCamera.h"

#include <iostream>
#include <ObjectArray.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../InputManager.h"

SceneViewCamera::SceneViewCamera(const std::shared_ptr<InputManager>& inputManager, glm::uvec2 screenSize, glm::vec3 position, glm::vec3 look, float zNear, float zFar, float fovy):
    m_screenSize(screenSize),
    m_aspectRatio(static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y)),
    m_zNear(zNear),
    m_zFar(zFar),
    m_fovy(fovy),
    m_position(position),
    m_look(look),
    m_up(glm::vec3(0, 1, 0)),
    m_viewMatrix(glm::mat4(1)),
    m_projectionMatrix(glm::mat4(1)),
    m_cameraMatrix(glm::mat4(1)),
    m_inputManager(inputManager),
    m_movementSpeed(1.f),
    m_movementDirection(glm::vec3(0))
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
    UpdateFramebuffer();
    
    m_inputManager->SubscribeToKeyEvents([this](const int key, const int action)
    {
        HandleKeyEvent(key, action);
    });
    
    m_inputManager->SubscribeToCursorPosEvents([this](const double xpos, const double ypos)
    {
        HandleCursorPosEvent(xpos, ypos);
    });

    m_inputManager->SubscribeToMouseButtonEvents([this](const int button, const int action)
    {
        HandleMouseButtonEvent(button, action);
    });
}

void SceneViewCamera::Update(double seconds)
{
    if (m_movementDirection.IsDirty())
    {
        UpdateMovementDirection();
    }
    if (m_movementDirection.GetData() != glm::vec3(0))
    {
        MoveCamera(seconds);
    }
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

const glm::mat4& SceneViewCamera::GetCameraMatrix()
{
    if (m_cameraMatrix.IsDirty())
    {
        UpdateCameraMatrix();
    }
    return m_cameraMatrix.GetData();
}

void SceneViewCamera::PrintCameraMatrix() const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::cout<<m_cameraMatrix.GetData()[i][j]<<", ";
        }
        std::cout<<"\n";
    }
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
    m_viewMatrix = glm::lookAt(m_position, m_position + m_look, m_up);
    m_cameraMatrix.MarkDirty();
}

void SceneViewCamera::UpdateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(m_fovy, m_aspectRatio, m_zNear, m_zFar);
    m_cameraMatrix.MarkDirty();
}

void SceneViewCamera::UpdateCameraMatrix()
{
    m_cameraMatrix = m_projectionMatrix * m_viewMatrix;
}

void SceneViewCamera::HandleKeyEvent(int key, int action)
{
    if (action == GLFW_PRESS)
    {
        m_movementDirection.MarkDirty();
    }
    else if (action == GLFW_RELEASE)
    {
        m_movementDirection.MarkDirty();
    }
}

void SceneViewCamera::HandleCursorPosEvent(double xpos, double ypos)
{
    // TODO: Rotate Camera
}

void SceneViewCamera::HandleMouseButtonEvent(int button, int action)
{
}

void SceneViewCamera::UpdateMovementDirection()
{
    glm::vec3 newMovementDirection = glm::vec3(0);
    glm::vec3 rightVector = glm::normalize(glm::cross(m_look, m_up));
    if (m_inputManager->IsKeyDown(GLFW_KEY_W))
    {
        newMovementDirection += m_look;
    }
    if (m_inputManager->IsKeyDown(GLFW_KEY_S))
    {
        newMovementDirection -= m_look;
    }
    if (m_inputManager->IsKeyDown(GLFW_KEY_A))
    {
        newMovementDirection -= rightVector;
    }
    if (m_inputManager->IsKeyDown(GLFW_KEY_D))
    {
        newMovementDirection += rightVector;
    }
    if (m_inputManager->IsKeyDown(GLFW_KEY_SPACE))
    {
        newMovementDirection += m_up;
    }
    if (m_inputManager->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
    {
        newMovementDirection -= m_up;
    }

    if (newMovementDirection != glm::vec3(0))
    {
        newMovementDirection = glm::normalize(newMovementDirection);
    }
    
    m_movementDirection.SetCleanData(newMovementDirection);
}

void SceneViewCamera::MoveCamera(double seconds)
{
    m_position += static_cast<float>(seconds) * m_movementSpeed * m_movementDirection.GetData();
    UpdateViewMatrix();
}
