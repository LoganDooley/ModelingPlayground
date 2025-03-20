#pragma once
#include "WindowManager.h"
#include "../OpenGLHelpers/OpenGLRenderer.h"

class ApplicationManager {
public:
	ApplicationManager();
	~ApplicationManager();

	int Run();

private:
	int Initialize();
	int Loop();
	int End();

	std::shared_ptr<SceneHierarchy> m_scene;
	std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
	std::unique_ptr<GlfwWindow> m_glfwWindow;
	std::unique_ptr<WindowManager> m_windowManager;
};
