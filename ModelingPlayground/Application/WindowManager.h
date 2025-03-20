#pragma once
#include "GlfwWindow.h"
#include "Window/Window.h"
#include <memory>
#include <vector>

#include "../OpenGLHelpers/OpenGLRenderer.h"
#include "../Scene/SceneHierarchy.h"

class WindowManager {
public:
	WindowManager();
	~WindowManager();

	void Initialize(std::unique_ptr<GlfwWindow>& glfwWindow, const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const std::shared_ptr<OpenGLRenderer>& openGLRenderer);
	void Update(double seconds) const;
	void Render(std::unique_ptr<GlfwWindow>& glfwWindow);
	void Close();

private:
	std::vector<std::shared_ptr<Window>> m_windows;
};
