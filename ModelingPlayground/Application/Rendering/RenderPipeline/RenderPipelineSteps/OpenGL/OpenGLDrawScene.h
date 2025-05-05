#pragma once
#include "../RenderPipelineDrawStep.h"

class OpenGLBufferManager;
class SceneViewCamera;
class OpenGLShader;
class OpenGLBuffer;

class OpenGLDrawScene : public RenderPipelineDrawStep
{
public:
    OpenGLDrawScene(const std::string& name, std::shared_ptr<DrawCommand> drawCommand,
                    std::shared_ptr<SceneViewCamera> camera,
                    std::shared_ptr<OpenGLShader> shader, std::shared_ptr<OpenGLBufferManager> bufferManager);
    ~OpenGLDrawScene() override = default;

    void PreExecute() const override;
    void PostExecute() const override;

private:
    std::shared_ptr<SceneViewCamera> m_camera;
    std::shared_ptr<OpenGLShader> m_shader;
    std::shared_ptr<OpenGLBufferManager> m_bufferManager;
};
