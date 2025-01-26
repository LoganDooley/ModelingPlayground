#pragma once
#include "GlfwWindow.h"
#include "Window/Window.h"
#include <memory>
#include <vector>

#include "../Scene/Scene.h"

class WindowManager {
public:
	WindowManager(const std::shared_ptr<Scene>& scene);
	~WindowManager();

	void Initialize(std::unique_ptr<GlfwWindow>& glfwWindow);
	void Render(std::unique_ptr<GlfwWindow>& glfwWindow);
	void Close();

private:
	std::vector<std::shared_ptr<Window>> m_windows;
	std::shared_ptr<Scene> m_scene;
};
