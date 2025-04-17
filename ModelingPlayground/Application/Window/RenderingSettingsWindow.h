#pragma once
#include "Window.h"

class RenderingSettingsWindow : public Window
{
public:
	RenderingSettingsWindow();

	void Render() override;
	void Update(double seconds) override;
};
