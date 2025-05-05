#include "RenderingManager.h"

#include "../../Utils/PropertyDrawer.h"
#include "Primitives/PrimitiveManager.h"
#include "Renderers/OpenGLRenderer.h"

RenderingManager::RenderingManager(std::shared_ptr<SceneHierarchy> sceneHierarchy):
    m_renderingMode(RenderingMode::Raster),
    m_primitiveManager(std::make_shared<PrimitiveManager>()),
    m_renderer(std::make_unique<OpenGLRenderer>(m_primitiveManager, sceneHierarchy)),
    m_sceneHierarchy(sceneHierarchy)
{
}

void RenderingManager::SetCamera(std::shared_ptr<SceneViewCamera>& sceneViewCamera)
{
    m_camera = sceneViewCamera;
    m_renderer->SetCamera(sceneViewCamera);
}

void RenderingManager::Render() const
{
    m_renderer->Render();
}

void RenderingManager::AddPrimitive(const std::string& filePath) const
{
    m_primitiveManager->AddPrimitive(filePath);
}

void RenderingManager::AddTexture(const std::string& filePath) const
{
    m_renderer->AddTexture(filePath);
}

void RenderingManager::IncrementTextureUsage(const std::string& filePath, void* user) const
{
    m_renderer->IncrementTextureUsage(filePath, user);
}

void RenderingManager::DecrementTextureUsage(const std::string& filePath, void* user) const
{
    m_renderer->DecrementTextureUsage(filePath, user);
}

const std::shared_ptr<OpenGLTextureCache>& RenderingManager::GetTextureCache() const
{
    return m_renderer->GetTextureCache();
}

std::vector<std::string> RenderingManager::GetPrimitiveNames() const
{
    return m_primitiveManager->GetPrimitiveNames();
}

void RenderingManager::DrawSettings()
{
    PropertyDrawer::DrawEnumCombo("Rendering Mode", m_renderingMode);
    m_renderer->DrawSettings();
}

void RenderingManager::SelectObjectAtPixel(int x, int y) const
{
    m_renderer->SelectObjectAtPixel(x, y);
}
