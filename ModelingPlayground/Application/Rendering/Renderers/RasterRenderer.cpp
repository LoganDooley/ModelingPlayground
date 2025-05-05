#include "RasterRenderer.h"

#include "../../Scene/SceneHierarchy.h"
#include "../RenderPipeline/RenderPipeline.h"

RasterRenderer::RasterRenderer(std::shared_ptr<PrimitiveManager> primitiveManager,
                               std::shared_ptr<SceneHierarchy> sceneHierarchy):
    Renderer(primitiveManager, sceneHierarchy),
    m_rasterPipeline(RasterPipeline::Forward),
    m_globalIllumination(GlobalIllumination::Ambient),
    m_ambientOcclusion(AmbientOcclusion::None),
    m_antiAliasing(AntiAliasing::None),
    m_renderPipeline(std::make_shared<RenderPipeline>())
{
}

RasterRenderer::~RasterRenderer()
{
}

void RasterRenderer::DrawSettings()
{
}

void RasterRenderer::Render() const
{
    m_renderPipeline->Render();
}
