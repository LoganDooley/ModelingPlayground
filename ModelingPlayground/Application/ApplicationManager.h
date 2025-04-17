#pragma once
#include "WindowManager.h"
#include "Rendering/Renderers/OpenGLRenderer.h"

class RenderingManager;

class ApplicationManager
{
public:
	ApplicationManager();
	~ApplicationManager();

	int Run();

private:
	int Initialize();
	int Loop();
	int End();

	std::shared_ptr<SceneHierarchy> m_scene;
	std::shared_ptr<RenderingManager> m_renderingManager;
	std::unique_ptr<GlfwWindow> m_glfwWindow;
	std::unique_ptr<WindowManager> m_windowManager;
};
