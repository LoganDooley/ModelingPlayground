#include "OpenGLDrawScene.h"

#include "../../../../../OpenGLHelpers/Shader/OpenGLShader.h"
#include "../../../../Window/SceneViewCamera.h"
#include "../../../Wrappers/BufferManagers/OpenGLBufferManager.h"

OpenGLDrawScene::OpenGLDrawScene(const std::string& name, std::shared_ptr<DrawCommand> drawCommand,
                                 std::shared_ptr<SceneViewCamera> camera, std::shared_ptr<OpenGLShader> shader,
                                 std::shared_ptr<OpenGLBufferManager> bufferManager):
    RenderPipelineDrawStep(name, drawCommand),
    m_camera(camera),
    m_shader(shader),
    m_bufferManager(bufferManager)
{
}

void OpenGLDrawScene::PreExecute() const
{
    m_camera->BindFramebuffer();
    m_camera->SetViewport();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader->BindShader();
    if (!m_bufferManager->GetModelMatrixBuffer() || !m_bufferManager->GetInverseTransposeModelMatrixBuffer() || !
        m_bufferManager->GetMaterialBuffer())
    {
        return;
    }
    m_bufferManager->GetModelMatrixBuffer()->BindBase(
        m_shader->GetShaderStorageBlock("ModelMatrixBuffer")->GetShaderStorageBlockBinding());
    m_bufferManager->GetInverseTransposeModelMatrixBuffer()->BindBase(
        m_shader->GetShaderStorageBlock("InverseTransposeModelMatrixBuffer")->GetShaderStorageBlockBinding());
    m_bufferManager->GetMaterialBuffer()->BindBase(
        m_shader->GetShaderStorageBlock("MaterialBuffer")->GetShaderStorageBlockBinding());
}

void OpenGLDrawScene::PostExecute() const
{
    m_camera->UnbindFramebuffer();
}
