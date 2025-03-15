#pragma once

#include <string>

class Window
{
public:
	virtual ~Window() = default;

	virtual void Render() = 0;
	virtual void Update(double seconds) = 0;
};

