#include "RasterRenderer.h"

#include "../../Scene/SceneHierarchy.h"

RasterRenderer::RasterRenderer(std::shared_ptr<PrimitiveManager> primitiveManager):
    Renderer(primitiveManager),
    m_rasterPipeline(RasterPipeline::Forward),
    m_globalIllumination(GlobalIllumination::Ambient),
    m_ambientOcclusion(AmbientOcclusion::None),
    m_antiAliasing(AntiAliasing::None)
{
}

RasterRenderer::~RasterRenderer()
{
}

void RasterRenderer::DrawSettings()
{
}
