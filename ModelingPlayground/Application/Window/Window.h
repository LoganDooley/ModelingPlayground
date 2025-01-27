#pragma once

#include <string>

class Window
{
public:
	virtual ~Window() = default;

	virtual void Render() = 0;
};

