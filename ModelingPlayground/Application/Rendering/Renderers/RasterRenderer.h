#pragma once
#include <glm/mat4x4.hpp>

#include "Renderer.h"

enum class RasterPipeline
{
    Forward, ForwardPlus, Deferred
};

enum class AmbientOcclusion
{
    None, SSAO
};

enum class AntiAliasing
{
    None, FXAA, MLAA, MSAA, SMAA, TAA, TSAA, CSAA
};

enum class GlobalIllumination
{
    None, Ambient, VoxelConeTracing
};

class RasterRenderer : public Renderer
{
public:
    RasterRenderer(std::shared_ptr<PrimitiveManager> primitiveManager);
    ~RasterRenderer() override;

    void DrawSettings() override;

    virtual void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const = 0;
    virtual void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const = 0;

protected:
    virtual void ClearCameraFramebuffer() const = 0;
    virtual void DepthPrepass() const = 0;
    virtual void SetAmbientLightColor() const = 0;

    RasterPipeline m_rasterPipeline;
    GlobalIllumination m_globalIllumination;
    AmbientOcclusion m_ambientOcclusion;
    AntiAliasing m_antiAliasing;
};
