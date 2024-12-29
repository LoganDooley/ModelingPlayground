#pragma once
#include "ImGuiManager.h"

class ApplicationManager {
public:
	ApplicationManager();
	~ApplicationManager();

	int Run();

private:
	int Initialize();
	int Loop();
	int End();

	std::unique_ptr<Window> m_window;
	std::unique_ptr<ImGuiManager> m_imGuiManager;
};