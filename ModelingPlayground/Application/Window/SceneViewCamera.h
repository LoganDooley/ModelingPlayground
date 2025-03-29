#pragma once
#include <memory>
#include <glm/mat4x4.hpp>

#include "../../Utils/LazyValue.h"
#include "glad/glad.h"

class OpenGLFramebuffer;
class InputManager;

class SceneViewCamera
{
public:
	SceneViewCamera(std::shared_ptr<InputManager> inputManager, glm::uvec2 screenSize,
	                glm::vec3 position = glm::vec3(0), glm::vec3 look = glm::vec3(0, 0, -1), float zNear = 0.1f,
	                float zFar = 100.f, float fovy = 1.f);

	void Update(double seconds);

	void BindFramebuffer() const;
	void SetViewport() const;
	GLuint GetFramebuffer() const;
	void SetScreenSize(glm::uvec2 screenSize);
	glm::uvec2 GetScreenSize() const;
	float GetAspectRatio() const;
	const glm::mat4& GetCameraMatrix();
	const glm::vec3& GetCameraPosition() const;

	void PrintCameraMatrix();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	glm::mat4 ComputeCameraMatrix() const;
	glm::vec3 ComputeMovementDirection();

	void HandleKeyEvent(int key, int action);
	void HandleCursorPosEvent(double xpos, double ypos);
	void HandleMouseButtonEvent(int button, int action);

	void MoveCamera(double seconds);

	glm::uvec2 m_screenSize;
	float m_aspectRatio;
	float m_zNear;
	float m_zFar;
	float m_fovy;

	glm::vec3 m_position;
	glm::vec3 m_look;
	glm::vec3 m_up;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	LazyValue<glm::mat4> m_cameraMatrix;

	std::shared_ptr<OpenGLFramebuffer> m_framebuffer;

	std::shared_ptr<InputManager> m_inputManager;
	float m_movementSpeed;

	LazyValue<glm::vec3> m_movementDirection;
};
