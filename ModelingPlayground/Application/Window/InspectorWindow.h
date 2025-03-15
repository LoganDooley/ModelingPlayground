#pragma once

#include <memory>

#include "Window.h"

class Scene;

class InspectorWindow : public Window
{
public:
	InspectorWindow(const std::shared_ptr<Scene>& scene);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Inspector";

private:
	std::shared_ptr<Scene> m_scene;
};

