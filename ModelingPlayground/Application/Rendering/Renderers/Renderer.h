#pragma once
#include <memory>
#include <string>
#include <vector>

class PrimitiveManager;
class SceneNode;
class SceneHierarchy;
class SceneViewCamera;
class OpenGLTextureCache;

class Renderer
{
public:
    Renderer(std::shared_ptr<PrimitiveManager> primitiveManager);
    virtual ~Renderer() = default;

    virtual void Initialize() = 0;
    virtual void SetCamera(std::shared_ptr<SceneViewCamera> sceneViewCamera) = 0;
    virtual void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy) = 0;

    virtual void DrawSettings() = 0;
    virtual void Render() const = 0;

    virtual void AddTexture(const std::string& filePath) const = 0;

    virtual void IncrementTextureUsage(const std::string& filePath, void* user) const = 0;
    virtual void DecrementTextureUsage(const std::string& filePath, void* user) const = 0;

    virtual const std::unique_ptr<OpenGLTextureCache>& GetTextureCache() const = 0;

    void SelectObjectAtPixel(int x, int y) const;

protected:
    virtual void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const = 0;

    std::shared_ptr<SceneViewCamera> m_camera;
    std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
    std::shared_ptr<PrimitiveManager> m_primitiveManager;
};
