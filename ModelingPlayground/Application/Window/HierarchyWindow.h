#pragma once

#include <memory>

#include "Window.h"

class SceneNode;
class ObjectSceneNode;
class Scene;

class HierarchyWindow : public Window
{
public:
	HierarchyWindow(const std::shared_ptr<Scene>& scene);

	void Render() override;

	inline static const std::string Name = "Hierarchy";

private:
	void DrawNode(const std::shared_ptr<SceneNode>& node);
	void CheckNodeSelection(const std::shared_ptr<SceneNode>& node);
	void CheckNodePopupMenu(const std::shared_ptr<SceneNode>& node);
	
	std::shared_ptr<Scene> m_scene;
};

