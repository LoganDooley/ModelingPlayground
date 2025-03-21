#include "ApplicationManager.h"

#include "../Utils/SceneLoader.h"

ApplicationManager::ApplicationManager():
	m_scene(std::make_shared<SceneHierarchy>()),
	m_openGLRenderer(std::make_shared<OpenGLRenderer>()),
	m_glfwWindow(std::make_unique<GlfwWindow>()),
	m_windowManager(std::make_unique<WindowManager>())
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
	m_openGLRenderer->Initialize();
	SceneLoader::LoadScene(m_scene, m_openGLRenderer);
	m_windowManager->Initialize(m_glfwWindow, m_scene, m_openGLRenderer);
	return 0;
}

int ApplicationManager::Loop()
{
	double time = glfwGetTime();
	while (!m_glfwWindow->ShouldClose()) {
		m_glfwWindow->PollEvents();
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		m_windowManager->Update(deltaTime);
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
