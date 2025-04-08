#include "SceneViewCamera.h"

#include <iostream>
#include <ObjectArray.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../InputManager.h"
#include "../../OpenGLHelpers/OpenGLFramebuffer.h"

SceneViewCamera::SceneViewCamera(std::shared_ptr<InputManager> inputManager, glm::uvec2 screenSize, glm::vec3 position,
                                 glm::vec3 look, float zNear, float zFar, float fovy):
	m_screenSize(screenSize),
	m_aspectRatio(static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y)),
	m_zNear(zNear),
	m_zFar(zFar),
	m_fovy(fovy),
	m_position(position),
	m_look(look),
	m_up(glm::vec3(0, 1, 0)),
	m_projectionMatrix(glm::mat4(1)),
	m_viewMatrix(glm::mat4(1)),
	m_cameraMatrix(glm::mat4(1), [this] { return ComputeCameraMatrix(); }),
	m_inputManager(inputManager),
	m_movementSpeed(1.f),
	m_rotationSpeed(1.f),
	m_movementDirection(glm::vec3(0), [this] { return ComputeMovementDirection(); })
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	m_framebuffer = std::make_shared<OpenGLFramebuffer>(m_screenSize.x, m_screenSize.y,
	                                                    std::vector<TextureAttachmentArguments>({
		                                                    {
			                                                    .m_attachment = GL_COLOR_ATTACHMENT0,
			                                                    .m_internalFormat = GL_RGB,
			                                                    .m_format = GL_RGB,
			                                                    .m_dataType = GL_UNSIGNED_BYTE,
			                                                    .m_textureParameterSettings = std::vector<
				                                                    TextureParameterSetting>({
				                                                    {GL_TEXTURE_MIN_FILTER, GL_LINEAR},
				                                                    {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
			                                                    }),
			                                                    .m_textureTarget = GL_TEXTURE_2D,
		                                                    }
	                                                    }),
	                                                    std::vector<RenderbufferAttachmentArguments>({
		                                                    {
			                                                    .m_attachment = GL_DEPTH_STENCIL_ATTACHMENT,
			                                                    .m_internalFormat = GL_DEPTH24_STENCIL8,
		                                                    }
	                                                    }));

	m_inputManager->SubscribeToKeyEvents([this](const int key, const int action)
	{
		HandleKeyEvent(key, action);
	});

	m_inputManager->SubscribeToCursorPosEvents(
		[this](const double xpos, const double ypos, const double deltaX, const double deltaY)
		{
			HandleCursorPosEvent(xpos, ypos, deltaX, deltaY);
		});

	m_inputManager->SubscribeToMouseButtonEvents([this](const int button, const int action)
	{
		HandleMouseButtonEvent(button, action);
	});
}

void SceneViewCamera::Update(double seconds)
{
	if (m_movementDirection.GetValue() != glm::vec3(0))
	{
		MoveCamera(seconds);
	}
}

void SceneViewCamera::BindFramebuffer() const
{
	m_framebuffer->Bind();
}

void SceneViewCamera::UnbindFramebuffer() const
{
	m_framebuffer->Unbind();
}

void SceneViewCamera::SetViewport() const
{
	glViewport(0, 0, static_cast<int>(m_screenSize.x), static_cast<int>(m_screenSize.y));
}

GLuint SceneViewCamera::GetFramebuffer() const
{
	return m_framebuffer->GetFramebufferId();
}

void SceneViewCamera::SetScreenSize(glm::uvec2 screenSize)
{
	if (m_screenSize != screenSize)
	{
		m_screenSize = screenSize;
		m_aspectRatio = static_cast<float>(m_screenSize.x) / static_cast<float>(m_screenSize.y);
		UpdateProjectionMatrix();
		m_framebuffer->Resize(screenSize.x, m_screenSize.y);
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
	return m_cameraMatrix.GetValue();
}

const glm::vec3& SceneViewCamera::GetCameraPosition() const
{
	return m_position;
}

void SceneViewCamera::PrintCameraMatrix()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << m_cameraMatrix.GetValue()[i][j] << ", ";
		}
		std::cout << "\n";
	}
}

void SceneViewCamera::UpdateViewMatrix()
{
	m_viewMatrix = lookAt(m_position, m_position + m_look, m_up);
	m_cameraMatrix.MarkDirty();
}

void SceneViewCamera::UpdateProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(m_fovy, m_aspectRatio, m_zNear, m_zFar);
	m_cameraMatrix.MarkDirty();
}

glm::mat4 SceneViewCamera::ComputeCameraMatrix() const
{
	return m_projectionMatrix * m_viewMatrix;
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

void SceneViewCamera::HandleCursorPosEvent(double xpos, double ypos, double deltaX, double deltaY)
{
	if (!m_inputManager->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		return;
	}
	float upRotation = -glm::radians(deltaX * m_rotationSpeed);
	float rightRotation = -glm::radians(deltaY * m_rotationSpeed);

	glm::vec3 right = cross(m_look, m_up);

	glm::mat4 rotationMatrix = rotate(rotate(glm::mat4(1.0f), upRotation, m_up), rightRotation, right);
	m_look = glm::vec3(rotationMatrix * glm::vec4(m_look, 0.0f));

	UpdateViewMatrix();
	if (m_movementDirection.GetValue() != glm::vec3(0))
	{
		m_movementDirection.MarkDirty();
	}
}

void SceneViewCamera::HandleMouseButtonEvent(int button, int action)
{
}

glm::vec3 SceneViewCamera::ComputeMovementDirection()
{
	auto newMovementDirection = glm::vec3(0);
	glm::vec3 rightVector = normalize(cross(m_look, m_up));
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
		newMovementDirection = normalize(newMovementDirection);
	}

	return newMovementDirection;
}

void SceneViewCamera::MoveCamera(double seconds)
{
	m_position += static_cast<float>(seconds) * m_movementSpeed * m_movementDirection.GetValue();
	UpdateViewMatrix();
}
