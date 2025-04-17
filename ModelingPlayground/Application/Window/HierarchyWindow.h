#pragma once

#include <memory>

#include "Window.h"

class RenderingManager;
class SceneNode;
class ObjectSceneNode;
class SceneHierarchy;
class SceneNodeGenerator;

class HierarchyWindow : public Window
{
public:
	HierarchyWindow(std::shared_ptr<SceneHierarchy> scene, std::shared_ptr<RenderingManager> renderingManager);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Hierarchy";

private:
	void DrawNode(const std::shared_ptr<SceneNode>& node);
	void CheckNodeSelection(const std::shared_ptr<SceneNode>& node);
	void CheckNodePopupMenu(const std::shared_ptr<SceneNode>& node);

	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
	std::shared_ptr<RenderingManager> m_renderingManager;
};
