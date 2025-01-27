#include "ApplicationManager.h"

ApplicationManager::ApplicationManager():
	m_scene(std::make_shared<Scene>()),
	m_glfwWindow(std::make_unique<GlfwWindow>()),
	m_windowManager(std::make_unique<WindowManager>(m_scene))
{

}

ApplicationManager::~ApplicationManager() {
	
}

int ApplicationManager::Run()
{
	int initializeCode = Initialize();
	if (initializeCode != 0) {
		return initializeCode;
	}
	int loopCode = Loop();
	if (loopCode != 0) {
		return loopCode;
	}
	int endCode = End();
	if (endCode != 0) {
		return endCode;
	}
	return 0;
}

int ApplicationManager::Initialize()
{
	m_glfwWindow->Initialize();
	m_windowManager->Initialize(m_glfwWindow);
	return 0;
}

int ApplicationManager::Loop()
{
	while (!m_glfwWindow->ShouldClose()) {
		m_glfwWindow->PollEvents();
		m_windowManager->Render(m_glfwWindow);
		m_glfwWindow->SwapBuffers();
	}
	return 0;
}

int ApplicationManager::End()
{
	m_glfwWindow->Close();
	return 0;
}
