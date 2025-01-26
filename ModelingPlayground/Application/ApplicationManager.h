#pragma once
#include "WindowManager.h"

class ApplicationManager {
public:
	ApplicationManager();
	~ApplicationManager();

	int Run();

private:
	int Initialize();
	int Loop();
	int End();

	std::shared_ptr<Scene> m_scene;
	std::unique_ptr<GlfwWindow> m_glfwWindow;
	std::unique_ptr<WindowManager> m_windowManager;
};
