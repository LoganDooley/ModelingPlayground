#include "ApplicationManager.h"

ApplicationManager::ApplicationManager():
	m_window(std::make_unique<Window>()),
	m_imGuiManager(std::make_unique<ImGuiManager>())
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
	m_window->Initialize();
	m_imGuiManager->Initialize(m_window);
	return 0;
}

int ApplicationManager::Loop()
{
	while (!m_window->ShouldClose()) {
		m_window->PollEvents();
		m_imGuiManager->Render(m_window);
		m_window->SwapBuffers();
	}
	return 0;
}

int ApplicationManager::End()
{
	m_window->Close();
	return 0;
}
