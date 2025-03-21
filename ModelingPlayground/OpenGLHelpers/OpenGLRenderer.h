#pragma once

#include "OpenGLLightContainer.h"
#include "OpenGLPrimitiveDrawer.h"
#include "../Scene/SceneHierarchy.h"

class MaterialComponent;
class PrimitiveComponent;
class TransformComponent;
class SceneViewCamera;

class OpenGLRenderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer() = default;

    void Initialize();

    void SetCamera(const std::shared_ptr<SceneViewCamera>& camera);
    void SetSceneHierarchy(const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
    
    void RenderSceneHierarchy() const;

private:
    void ProcessObject(const Object& object, glm::mat4& cumulativeModelMatrix) const;
    void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
                  	const MaterialComponent& materialComponent, glm::mat4& cumulativeModelMatrix) const;

    void CacheSceneHierarchyLights() const;
    void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const;
    
    std::shared_ptr<OpenGLShader> m_defaultShader;
    std::shared_ptr<SceneViewCamera> m_camera;
    std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
    std::unique_ptr<OpenGLPrimitiveDrawer> m_openGLPrimitiveDrawer;
    std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
};
