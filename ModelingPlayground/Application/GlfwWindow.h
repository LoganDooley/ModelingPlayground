#pragma once
#define GLFW_INCLUDE_NONE
#include <memory>
#include <GLFW/glfw3.h>

#include "InputManager.h"

class GlfwWindow {
public:
	GlfwWindow();
	~GlfwWindow();

	int Initialize();
	bool ShouldClose() const;
	void PollEvents();
	void SwapBuffers() const;
	void Close();

	GLFWwindow* GetWindowPointer() const;
	const std::shared_ptr<InputManager>& GetInputManager() const;

private:
	
	GLFWwindow* m_window;
	std::shared_ptr<InputManager> m_inputManager;
};
