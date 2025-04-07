#pragma once
#include <functional>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Initialize(GLFWwindow* window);

	void SubscribeToKeyEvents(std::function<void(int, int)> callback);
	void SubscribeToCursorPosEvents(std::function<void(double, double, double, double)> callback);
	void SubscribeToMouseButtonEvents(std::function<void(int, int)> callback);

	void SetSceneViewPos(glm::vec2 pos);
	glm::vec2 GetSceneViewCursorPos() const;

	bool IsKeyDown(int key) const;
	bool IsMouseButtonDown(int mouseButton) const;

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void UpdateKeyState(int key, bool down);
	void UpdateMouseButtonState(int mouseButton, bool down);

	std::vector<std::function<void(int, int)>> m_keyEventSubscribers;
	std::vector<std::function<void(double, double, double, double)>> m_cursorPosEventSubscribers;
	std::vector<std::function<void(int, int)>> m_mouseButtonEventSubscribers;
	std::unordered_set<int> m_keysDown;
	std::unordered_set<int> m_mouseButtonsDown;
	glm::vec2 m_cursorPos;
	glm::vec2 m_sceneViewPos;
};
