#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class Window {
public:
	Window();
	~Window();

	int Initialize();
	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
	void Close();

	GLFWwindow* GetWindowPointer();

private:
	GLFWwindow* m_window;
};