#pragma once
#include <memory>
#include <vector>
#include "GlfwWindow.h"
#include "Window/Window.h"

#include "../Scene/SceneHierarchy.h"

class RenderingManager;

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void Initialize(const std::unique_ptr<GlfwWindow>& glfwWindow, std::shared_ptr<SceneHierarchy> sceneHierarchy,
	                std::shared_ptr<RenderingManager> renderingManager);
	void Update(double seconds) const;
	void Render(const std::unique_ptr<GlfwWindow>& glfwWindow) const;
	void Close();

private:
	std::vector<std::shared_ptr<Window>> m_windows;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
	std::shared_ptr<RenderingManager> m_renderingManager;
};
