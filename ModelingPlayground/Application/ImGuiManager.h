#pragma once
#include "Window.h"
#include <memory>

class ImGuiManager {
public:
	ImGuiManager();
	~ImGuiManager();

	void Initialize(std::unique_ptr<Window>& window);
	void Render(std::unique_ptr<Window>& window);
	void Close();
};