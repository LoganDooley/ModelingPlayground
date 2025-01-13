#pragma once
#include "GlfwWindow.h"
#include "Window/Window.h"
#include <memory>
#include <vector>

class WindowManager {
public:
	WindowManager();
	~WindowManager();

	void Initialize(std::unique_ptr<GlfwWindow>& glfwWindow);
	void Render(std::unique_ptr<GlfwWindow>& glfwWindow);
	void Close();

private:
	std::vector<std::shared_ptr<Window>> m_windows;
};