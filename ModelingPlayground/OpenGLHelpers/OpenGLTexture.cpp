#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <ostream>
#include <unordered_map>

#include "../Libraries/stb/stb_image.h"

OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, GLint internalFormat, GLenum format,
                             GLenum dataType, std::vector<TextureParameterSetting> textureParameterSettings,
                             GLenum textureTarget):
    m_textureId(0),
    m_textureHandle(0),
    m_textureTarget(textureTarget)
{
    glGenTextures(1, &m_textureId);
    Bind();

    if (m_textureTarget == GL_TEXTURE_2D)
    {
        glTexImage2D(m_textureTarget, 0, internalFormat, width, height, 0, format, dataType, nullptr);

        for (const auto& textureParameterSetting : textureParameterSettings)
        {
            glTexParameteri(m_textureTarget, textureParameterSetting.m_texParameter,
                            textureParameterSetting.m_texParameterValue);
        }
    }
    else if (m_textureTarget == GL_TEXTURE_CUBE_MAP)
    {
        for (int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, dataType,
                         nullptr);
        }

        for (const auto& textureParameterSetting : textureParameterSettings)
        {
            glTexParameteri(m_textureTarget, textureParameterSetting.m_texParameter,
                            textureParameterSetting.m_texParameterValue);
        }
    }
    Unbind();

    m_textureHandle = glGetTextureHandleARB(m_textureId);

    MakeTextureNonResident();
}

OpenGLTexture::OpenGLTexture(const std::string& fileName):
    m_textureId(0),
    m_textureHandle(0),
    m_textureTarget(GL_TEXTURE_2D)
{
    glGenTextures(1, &m_textureId);
    std::unordered_map<int, GLenum> channelsToFormat = {
        {1, GL_RED},
        {2, GL_RG},
        {3, GL_RGB},
        {4, GL_RGBA}
    };
    std::unordered_map<int, GLint> channelsToInternalFormat = {
        {1, GL_RED},
        {2, GL_RG},
        {3, GL_RGB},
        {4, GL_RGBA}
    };

    int width;
    int height;
    int components;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &components, STBI_default);

    if (image == nullptr)
    {
        std::cerr << "OpenGLTexture: Failed to load image: " << fileName << "\n";
        glDeleteTextures(1, &m_textureId);
        return;
    }

    if (!channelsToFormat.contains(components))
    {
        std::cerr << "OpenGLTexture: Image has unsupported number of components: " << components << "\n";
        glDeleteTextures(1, &m_textureId);
        return;
    }

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, channelsToInternalFormat[components], width, height, 0, channelsToFormat[components],
                 GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Unbind();
    stbi_image_free(image);

    m_textureHandle = glGetTextureHandleARB(m_textureId);

    MakeTextureNonResident();
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &m_textureId);
}

GLuint OpenGLTexture::GetTextureId() const
{
    return m_textureId;
}

GLuint64 OpenGLTexture::GetTextureHandle() const
{
    return m_textureHandle;
}

void OpenGLTexture::MakeTextureResident()
{
    if (!glIsTextureHandleResidentARB(m_textureHandle))
    {
        glMakeTextureHandleResidentARB(m_textureHandle);
    }
}

void OpenGLTexture::MakeTextureNonResident()
{
    if (glIsTextureHandleResidentARB(m_textureHandle))
    {
        glMakeTextureHandleNonResidentARB(m_textureHandle);
    }
}

void OpenGLTexture::Bind()
{
    glBindTexture(m_textureTarget, m_textureId);
}

void OpenGLTexture::Unbind()
{
    glBindTexture(m_textureTarget, 0);
}
