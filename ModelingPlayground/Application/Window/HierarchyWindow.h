#pragma once

#include <memory>

#include "Window.h"
#include "../../Scene/SceneNode/SceneNodeGenerator.h"

class SceneNode;
class ObjectSceneNode;
class SceneHierarchy;

class HierarchyWindow : public Window
{
public:
	HierarchyWindow(const std::shared_ptr<SceneHierarchy>& scene);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Hierarchy";

private:
	void DrawNode(const std::shared_ptr<SceneNode>& node);
	void CheckNodeSelection(const std::shared_ptr<SceneNode>& node);
	void CheckNodePopupMenu(SceneNode& node);
	void TryRegisterSceneNodeAsLight(const std::shared_ptr<SceneNode>& node, SceneNodeType sceneNodeType);
	
	std::shared_ptr<SceneHierarchy> m_scene;
};

