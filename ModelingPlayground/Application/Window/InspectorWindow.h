#pragma once

#include "Window.h"

class InspectorWindow : public Window
{
public:
	InspectorWindow();
	~InspectorWindow();

	void Render() override;

	inline static const std::string Name = "Inspector";
};

