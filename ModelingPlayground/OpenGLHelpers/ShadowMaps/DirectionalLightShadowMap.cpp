#include "DirectionalLightShadowMap.h"

#include "../OpenGLFramebuffer.h"

#include "glad/glad.h"

DirectionalLightShadowMap::DirectionalLightShadowMap(unsigned int resolution)
{
	m_shadowMapFramebuffer = std::make_shared<OpenGLFramebuffer>(resolution, resolution,
	                                                             std::vector<TextureAttachmentArguments>({
		                                                             {
			                                                             .m_attachment = GL_DEPTH_ATTACHMENT,
			                                                             .m_internalFormat = GL_DEPTH_COMPONENT,
			                                                             .m_format = GL_DEPTH_COMPONENT,
			                                                             .m_dataType = GL_FLOAT,
			                                                             .m_textureParameterSettings = std::vector<
				                                                             TextureParameterSetting>({
				                                                             {GL_TEXTURE_MIN_FILTER, GL_NEAREST},
				                                                             {GL_TEXTURE_MAG_FILTER, GL_NEAREST},
				                                                             {GL_TEXTURE_WRAP_S, GL_REPEAT},
				                                                             {GL_TEXTURE_WRAP_T, GL_REPEAT}
			                                                             })
		                                                             }
	                                                             }),
	                                                             std::vector<RenderbufferAttachmentArguments>());
}

DirectionalLightShadowMap::~DirectionalLightShadowMap()
{
}

void DirectionalLightShadowMap::CaptureShadowMap(std::shared_ptr<SceneViewCamera> camera,
                                                 std::shared_ptr<OpenGLRenderer> openGLRenderer)
{
}
