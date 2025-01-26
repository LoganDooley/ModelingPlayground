#pragma once

#include <memory>

#include "Window.h"

class SceneObjectNode;
class Scene;

class HierarchyWindow : public Window
{
public:
	HierarchyWindow(const std::shared_ptr<Scene>& scene);
	~HierarchyWindow() override;

	void Render() override;

	inline static const std::string Name = "Hierarchy";\

private:
	void DrawNode(const std::shared_ptr<SceneObjectNode>& node);
	void CheckNodeSelection(const std::shared_ptr<SceneObjectNode>& node);
	void CheckNodePopupMenu(const std::shared_ptr<SceneObjectNode>& node);
	
	std::shared_ptr<Scene> m_scene;
};

