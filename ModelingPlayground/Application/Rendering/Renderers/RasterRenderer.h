#pragma once
#include <glm/mat4x4.hpp>

#include "Renderer.h"

#include "../RenderingManager.h"

class RasterRenderer : public Renderer
{
public:
	RasterRenderer();
	~RasterRenderer() override;

	void DrawSettings() override;

	virtual void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const = 0;
	virtual void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const = 0;

protected:
	virtual void ClearCameraFramebuffer() const = 0;
	virtual void DepthPrepass() const = 0;
	virtual void SetAmbientLightColor() const = 0;

	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;

	RasterPipeline m_rasterPipeline;
	GlobalIllumination m_globalIllumination;
	AmbientOcclusion m_ambientOcclusion;
	AntiAliasing m_antiAliasing;
};
