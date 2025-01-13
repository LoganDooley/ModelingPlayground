#pragma once

#include "Window.h"

class HierarchyWindow : public Window
{
public:
	HierarchyWindow();
	~HierarchyWindow();

	void Render() override;

	inline static const std::string Name = "Hierarchy";
};

