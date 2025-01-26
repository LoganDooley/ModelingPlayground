#pragma once

#include <string>

class Window
{
public:
	Window() {};
	virtual ~Window() {};

	virtual void Render() = 0;
};

