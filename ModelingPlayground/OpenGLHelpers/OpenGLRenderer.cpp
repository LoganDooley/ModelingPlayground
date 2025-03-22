#include "OpenGLRenderer.h"

#include <stack>

#include "../Application/Window/SceneViewCamera.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../../Scene/Components/MaterialComponent.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "../../Scene/Object.h"

OpenGLRenderer::OpenGLRenderer():
    m_defaultShader(std::make_shared<OpenGLShader>()),
    m_sceneHierarchy(std::make_shared<SceneHierarchy>()),
    m_openGLPrimitiveManager(std::make_shared<OpenGLPrimitiveManager>()),
    m_openGLLightContainer(std::make_unique<OpenGLLightContainer>())
{

}

void OpenGLRenderer::Initialize()
{
    m_defaultShader->LoadShader("Shaders/default.vert", "Shaders/default.frag");
    m_openGLPrimitiveManager->GeneratePrimitives(10, 10);
    m_openGLLightContainer->Initialize(m_defaultShader, 8);

    // Initialize shader
    m_defaultShader->RegisterUniformVariable("modelMatrix");
    m_defaultShader->RegisterUniformVariable("inverseTransposeModelMatrix");
    m_defaultShader->RegisterUniformVariable("cameraMatrix");
    m_defaultShader->RegisterUniformVariable("cameraPosition");
    m_defaultShader->RegisterUniformVariable("ambientColor");
    m_defaultShader->RegisterUniformVariable("materialColor");
    m_defaultShader->RegisterUniformVariable("roughness");
    m_defaultShader->RegisterUniformVariable("metallic");

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::SetCamera(std::shared_ptr<SceneViewCamera> camera)
{
    m_camera = camera;
}

void OpenGLRenderer::SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy)
{
    m_sceneHierarchy = sceneHierarchy;

    m_openGLLightContainer->SetSceneHierarchy(m_sceneHierarchy);
    
    m_sceneHierarchy->SubscribeToSceneNodeAdded([this](const std::shared_ptr<SceneNode>& newSceneNode)
    {
        OnSceneNodeAdded(newSceneNode);
    });
}

void OpenGLRenderer::RenderSceneHierarchy() const
{
    // Switch to offscreen framebuffer
    m_camera->BindFramebuffer();
    // Adjust the viewport size
    m_camera->SetViewport();

    // Bind shader
    m_defaultShader->BindShader();

    // Set camera uniforms
    // TODO: Only set these if dirty
    m_defaultShader->SetUniformMatrix4f("cameraMatrix", false, m_camera->GetCameraMatrix());
    m_defaultShader->SetUniform3f("cameraPosition", m_camera->GetCameraPosition());

    // DFS draw objects
    std::stack<std::pair<std::shared_ptr<SceneNode>, glm::mat4>> traversal;
    traversal.push({m_sceneHierarchy->GetRootSceneNode(), glm::mat4(1.0f)});
    while (!traversal.empty())
    {
        std::pair<std::shared_ptr<SceneNode>, glm::mat4> sceneNodeToProcess = traversal.top();
        std::shared_ptr<SceneNode> sceneNode = sceneNodeToProcess.first;
        glm::mat4 cumulativeModelMatrix = sceneNodeToProcess.second;
        traversal.pop();
        ProcessObject(sceneNode->GetObject(), cumulativeModelMatrix);

        // Add children to stack in reverse order
        const std::vector<std::shared_ptr<SceneNode>>& children = sceneNode->GetChildren();
        for (int i = static_cast<int>(children.size()) - 1; i >= 0; i--)
        {
            traversal.push({children[i], cumulativeModelMatrix});
        }
    }

    // Restore framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<OpenGLPrimitiveManager> OpenGLRenderer::GetOpenGLPrimitiveManager() const
{
    return m_openGLPrimitiveManager;
}

void OpenGLRenderer::ProcessObject(const Object& object, glm::mat4& cumulativeModelMatrix) const
{
    if (std::shared_ptr<PrimitiveComponent> primitiveComponent = object.GetFirstComponentOfType<PrimitiveComponent>())
    {
        if (std::shared_ptr<TransformComponent> transformComponent = object.GetFirstComponentOfType<TransformComponent>())
        {
            if (std::shared_ptr<MaterialComponent> materialComponent = object.GetFirstComponentOfType<MaterialComponent>())
            {
                DrawMesh(*primitiveComponent, *transformComponent, *materialComponent, cumulativeModelMatrix);
            }
        }
    }

    if (std::shared_ptr<OpenGLSettingsComponent> openGLSettingsComponent = object.GetFirstComponentOfType<OpenGLSettingsComponent>())
    {
        glm::vec4 clearColor = openGLSettingsComponent->GetClearColor();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_defaultShader->SetUniform3f("ambientColor", openGLSettingsComponent->GetAmbientLight());
    }
}

void OpenGLRenderer::DrawMesh(const PrimitiveComponent& primitiveComponent,
    const TransformComponent& transformComponent, const MaterialComponent& materialComponent,
    glm::mat4& cumulativeModelMatrix) const
{
    cumulativeModelMatrix = cumulativeModelMatrix * transformComponent.GetModelMatrix();
    m_defaultShader->BindShader();
    m_defaultShader->SetUniformMatrix4f("modelMatrix", false, cumulativeModelMatrix);
    glm::mat3 inverseTransposeModelMatrix = glm::transpose(glm::inverse(glm::mat3(cumulativeModelMatrix)));
    m_defaultShader->SetUniformMatrix3f("inverseTransposeModelMatrix", false, inverseTransposeModelMatrix);
    m_defaultShader->SetUniform3f("materialColor", materialComponent.GetMaterialColor());
    m_defaultShader->SetUniform1f("roughness", materialComponent.GetRoughness());
    m_defaultShader->SetUniform1f("metallic", materialComponent.GetRoughness());

    std::string customPrimitiveFilePath = primitiveComponent.GetCustomPrimitiveFilePath();
    if (!customPrimitiveFilePath.empty())
    {
        m_openGLPrimitiveManager->DrawPrimitive(customPrimitiveFilePath);
    }
    else
    {
        m_openGLPrimitiveManager->DrawPrimitive(primitiveComponent.GetPrimitiveType());
    }
}

void OpenGLRenderer::OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const
{
    m_openGLLightContainer->TryAddLight(newSceneNode);
}
