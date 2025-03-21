#pragma once

#include <memory>

#include "Window.h"

class SceneHierarchy;

class InspectorWindow : public Window
{
public:
	InspectorWindow(std::shared_ptr<SceneHierarchy> scene);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Inspector";

private:
	std::shared_ptr<SceneHierarchy> m_scene;
};

