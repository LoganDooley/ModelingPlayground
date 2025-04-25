#pragma once
#include <memory>
#include <string>
#include <vector>

class SceneHierarchy;
class SceneViewCamera;
class MaterialComponent;
class OpenGLTextureCache;
class Renderer;

enum class RenderingMode : int
{
    Raster = 0, Pathtraced = 1
};

/*
 *	This class stores rendering settings and an instance of a renderer
 */
class RenderingManager
{
public:
    RenderingManager();

    void Initialize() const;
    void SetCamera(std::shared_ptr<SceneViewCamera>& sceneViewCamera);
    void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy);

    void Render() const;

    void AddPrimitive(const std::string& filePath) const;
    void AddTexture(const std::string& filePath) const;

    void IncrementTextureUsage(const std::string& filePath, void* user) const;
    void DecrementTextureUsage(const std::string& filePath, void* user) const;

    const std::unique_ptr<OpenGLTextureCache>& GetTextureCache() const;

    std::vector<std::string> GetPrimitiveNames() const;

    void DrawSettings();

    void SelectObjectAtPixel(int x, int y) const;

private:
    RenderingMode m_renderingMode;
    std::unique_ptr<Renderer> m_renderer;
    std::shared_ptr<SceneViewCamera> m_camera;
};
