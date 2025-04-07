#include "InputManager.h"

#include <iostream>
#include <ostream>

#include "imgui.h"

InputManager::InputManager():
	m_keyEventSubscribers(std::vector<std::function<void(int, int)>>()),
	m_cursorPosEventSubscribers(std::vector<std::function<void(double, double, double, double)>>()),
	m_mouseButtonEventSubscribers(std::vector<std::function<void(int, int)>>()),
	m_keysDown(std::unordered_set<int>()),
	m_cursorPos(glm::vec2(0)),
	m_sceneViewPos(glm::vec2(0))
{
}

InputManager::~InputManager()
{
}

void InputManager::Initialize(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void InputManager::SubscribeToKeyEvents(std::function<void(int, int)> callback)
{
	m_keyEventSubscribers.push_back(callback);
}

void InputManager::SubscribeToCursorPosEvents(std::function<void(double, double, double, double)> callback)
{
	m_cursorPosEventSubscribers.push_back(callback);
}

void InputManager::SubscribeToMouseButtonEvents(std::function<void(int, int)> callback)
{
	m_mouseButtonEventSubscribers.push_back(callback);
}

void InputManager::SetSceneViewPos(glm::vec2 pos)
{
	m_sceneViewPos = pos;
}

glm::vec2 InputManager::GetSceneViewCursorPos() const
{
	ImVec2 mousePos = ImGui::GetMousePos();
	return glm::vec2(mousePos.x, mousePos.y) - m_sceneViewPos;
}

bool InputManager::IsKeyDown(int key) const
{
	return m_keysDown.contains(key);
}

bool InputManager::IsMouseButtonDown(int mouseButton) const
{
	return m_mouseButtonsDown.contains(mouseButton);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

	// Update keys down
	if (action == GLFW_PRESS)
	{
		inputManager->UpdateKeyState(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		inputManager->UpdateKeyState(key, false);
	}

	// Call subscribers
	for (const auto& subscriber : inputManager->m_keyEventSubscribers)
	{
		subscriber(key, action);
	}
}

void InputManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	auto inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	glm::vec2 delta = glm::vec2(xpos, ypos) - inputManager->m_cursorPos;
	inputManager->m_cursorPos = glm::vec2(xpos, ypos);
	for (const auto& subscriber : inputManager->m_cursorPosEventSubscribers)
	{
		subscriber(xpos, ypos, delta.x, delta.y);
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

	// Update mouse buttons down
	if (action == GLFW_PRESS)
	{
		inputManager->UpdateMouseButtonState(button, true);
	}
	else if (action == GLFW_RELEASE)
	{
		inputManager->UpdateMouseButtonState(button, false);
	}

	for (const auto& subscriber : inputManager->m_mouseButtonEventSubscribers)
	{
		subscriber(button, action);
	}
}

void InputManager::UpdateKeyState(int key, bool down)
{
	if (down)
	{
		m_keysDown.insert(key);
	}
	else
	{
		m_keysDown.erase(key);
	}
}

void InputManager::UpdateMouseButtonState(int mouseButton, bool down)
{
	if (down)
	{
		m_mouseButtonsDown.insert(mouseButton);
	}
	else
	{
		m_mouseButtonsDown.erase(mouseButton);
	}
}
