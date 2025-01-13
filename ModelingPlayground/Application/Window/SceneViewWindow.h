#pragma once

#include "Window.h"

class SceneViewWindow : public Window
{
public:
	SceneViewWindow();
	~SceneViewWindow();

	void Render() override;

	inline static const std::string Name = "Scene View";
};

