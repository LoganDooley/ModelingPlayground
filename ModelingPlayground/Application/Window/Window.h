#pragma once

#include <string>

class Window
{
public:
	Window() {};
	~Window() {};

	virtual void Render() = 0;
};

