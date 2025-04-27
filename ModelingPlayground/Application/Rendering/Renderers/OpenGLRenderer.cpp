#include "OpenGLRenderer.h"

#include <iostream>
#include <queue>
#include <stack>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../../OpenGLHelpers/OpenGLTexture.h"
#include "../../Application/Window/SceneViewCamera.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/MaterialComponent.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../Primitives/PrimitiveManager.h"
#include "../RenderPipeline/DrawCommand/OpenGLMultiDrawElementsCommand.h"
#include "../Wrappers/OpenGLBuffer.h"
#include "../Wrappers/VertexArray/OpenGLVertexArray.h"

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<PrimitiveManager> primitiveManager):
    RasterRenderer(primitiveManager),
    m_defaultShader(std::make_shared<OpenGLShader>()),
    m_depthShader(std::make_shared<OpenGLShader>()),
    m_omnidirectionalDepthShader(std::make_shared<OpenGLShader>()),
    m_openGLLightContainer(std::make_unique<OpenGLLightContainer>()),
    m_openGLTextureCache(std::make_unique<OpenGLTextureCache>())
{
    m_primitiveManager->SubscribeToOnPrimitiveAdded([this]
    {
        RebuildSceneMeshBuffers();
        RebuildSceneMultiDrawElementsCommand();
    });
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

void OpenGLRenderer::Initialize()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);

    m_defaultShader->LoadShader("Shaders/default.vert", "Shaders/default.frag");

    m_depthShader->LoadShader("Shaders/depth.vert",
                              "Shaders/depth.frag");

    m_omnidirectionalDepthShader->LoadShader("Shaders/omnidirectionalDepth.vert", "Shaders/omnidirectionalDepth.geom",
                                             "Shaders/omnidirectionalDepth.frag");

    m_openGLLightContainer->Initialize(m_defaultShader);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void OpenGLRenderer::ResetOpenGLTextureCache(OpenGLTextureCache* openGLTextureCache)
{
    m_openGLTextureCache.reset(openGLTextureCache);
}

void OpenGLRenderer::Render() const
{
    m_openGLLightContainer->UpdateDirtyShadowMaps(this);
    ClearCameraFramebuffer();
    DepthPrepass();
    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::RenderScene() const
{
    // Switch to offscreen framebuffer
    m_camera->BindFramebuffer();
    // Adjust the viewport size
    m_camera->SetViewport();

    // Set camera uniforms
    m_defaultShader->SetUniform<glm::mat4>("cameraMatrix", false, m_camera->GetCameraMatrix());
    m_defaultShader->SetUniform<glm::vec3>("cameraPosition", m_camera->GetCameraPosition());

    SetAmbientLightColor();

    // Set back face culling
    glCullFace(GL_BACK);

    RenderSceneHierarchy(m_defaultShader);
}

void OpenGLRenderer::RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const
{
    glCullFace(GL_FRONT);

    m_depthShader->SetUniform<glm::mat4>("cameraMatrix", false, lightMatrix);

    RenderSceneHierarchy(m_depthShader);
}

void OpenGLRenderer::RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const
{
    glCullFace(GL_FRONT);

    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.f, 0.1f, 100.f);

    const glm::mat4 matrix0 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix0", false, matrix0);

    const glm::mat4 matrix1 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix1", false, matrix1);

    const glm::mat4 matrix2 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix2", false, matrix2);

    const glm::mat4 matrix3 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix3", false, matrix3);

    const glm::mat4 matrix4 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix4", false, matrix4);

    const glm::mat4 matrix5 = shadowProjection *
        lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
    m_omnidirectionalDepthShader->SetUniform<glm::mat4>("lightMatrix5", false, matrix5);

    m_omnidirectionalDepthShader->SetUniform<glm::vec3>("lightPosition", lightPosition);
    m_omnidirectionalDepthShader->SetUniform<float>("farPlane", 100.f);

    RenderSceneHierarchy(m_omnidirectionalDepthShader);
}

void OpenGLRenderer::AddTexture(const std::string& filePath) const
{
    m_openGLTextureCache->AddTexture(filePath);
}

void OpenGLRenderer::IncrementTextureUsage(const std::string& filePath, void* user) const
{
    m_openGLTextureCache->IncrementTextureUsage(filePath, user);
}

void OpenGLRenderer::DecrementTextureUsage(const std::string& filePath, void* user) const
{
    m_openGLTextureCache->DecrementTextureUsage(filePath, user);
}

const std::unique_ptr<OpenGLTextureCache>& OpenGLRenderer::GetTextureCache() const
{
    return m_openGLTextureCache;
}

void OpenGLRenderer::RenderSceneHierarchy(const std::shared_ptr<OpenGLShader>& activeShader) const
{
    activeShader->BindShader();

    // DFS draw objects
    std::stack<std::shared_ptr<SceneNode>> traversal;
    traversal.push(m_sceneHierarchy->GetRootSceneNode());
    while (!traversal.empty())
    {
        std::shared_ptr<SceneNode> sceneNodeToProcess = traversal.top();
        traversal.pop();
        ProcessObject(sceneNodeToProcess->GetObject(), activeShader);

        // Add children to stack in reverse order
        const std::vector<std::shared_ptr<SceneNode>>& children = sceneNodeToProcess->GetChildren();
        for (int i = static_cast<int>(children.size()) - 1; i >= 0; i--)
        {
            traversal.push(children[i]);
        }
    }
}

void OpenGLRenderer::RebuildSceneMeshBuffers()
{
    m_primitiveToIndexMap.clear();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexOffset = 0;
    unsigned int indexOffset = 0;
    for (const auto& primitive : m_primitiveManager->GetAllPrimitives())
    {
        vertices.insert(vertices.end(), primitive.second->GetVertices().begin(), primitive.second->GetVertices().end());
        indices.insert(indices.end(), primitive.second->GetIndices().begin(), primitive.second->GetIndices().end());
        m_primitiveToIndexMap[primitive.first] = {vertexOffset, indexOffset};
        vertexOffset += primitive.second->GetVertices().size() / 8;
        indexOffset += primitive.second->GetIndices().size();
    }
    m_sceneVertexBuffer = std::make_shared<OpenGLBuffer>(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_sceneVertexArray = std::make_shared<OpenGLVertexArray>(m_sceneVertexBuffer,
                                                             std::vector({
                                                                 VertexAttribute::PositionF3, VertexAttribute::NormalF3,
                                                                 VertexAttribute::UVF2
                                                             }));
    m_sceneVertexArray->Bind();
    m_sceneIndexBuffer = std::make_shared<OpenGLBuffer>(indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    OpenGLVertexArray::Unbind();
}

void OpenGLRenderer::RebuildSceneMultiDrawElementsCommand()
{
    std::vector<DrawElementsIndirectCommand> drawCommands;
    m_sceneHierarchy->BreadthFirstProcessAllSceneNodes([this, drawCommands](std::shared_ptr<SceneNode> node) mutable
    {
        std::shared_ptr<PrimitiveComponent> primitiveComponent = node->GetObject().GetFirstComponentOfType<
            PrimitiveComponent>();
        if (primitiveComponent == nullptr)
        {
            return;
        }

        std::shared_ptr<Primitive> primitive = m_primitiveManager->GetPrimitive(primitiveComponent->GetPrimitiveName());
        drawCommands.emplace_back(DrawElementsIndirectCommand{
            .count = static_cast<unsigned int>(primitive->GetIndices().size()),
            .instanceCount = 1,
            .firstIndex = m_primitiveToIndexMap[primitiveComponent->GetPrimitiveName()].second,
            .baseVertex = m_primitiveToIndexMap[primitiveComponent->GetPrimitiveName()].first,
            .baseInstance = 0,
        });
    });

    m_sceneMultiDrawElementsCommand = std::make_shared<OpenGLMultiDrawElementsCommand>(
        m_sceneVertexArray, drawCommands);
}

void OpenGLRenderer::ClearCameraFramebuffer() const
{
    // Switch to offscreen framebuffer
    m_camera->BindFramebuffer();

    // Adjust the viewport size
    m_camera->SetViewport();

    std::shared_ptr<SceneNode> rootSceneNode = m_sceneHierarchy->GetRootSceneNode();
    if (rootSceneNode != nullptr)
    {
        std::shared_ptr<OpenGLSettingsComponent> openGLSettingsComponent = rootSceneNode->GetObject().
            GetFirstComponentOfType<OpenGLSettingsComponent>();
        if (openGLSettingsComponent != nullptr)
        {
            glm::vec4 clearColor = openGLSettingsComponent->GetClearColor();
            glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }
}

void OpenGLRenderer::DepthPrepass() const
{
    // Switch to offscreen framebuffer
    m_camera->BindFramebuffer();
    // Adjust the viewport size
    m_camera->SetViewport();

    // Set camera uniforms
    m_depthShader->SetUniform<glm::mat4>("cameraMatrix", false, m_camera->GetCameraMatrix());

    RenderSceneHierarchy(m_depthShader);
}

void OpenGLRenderer::SetAmbientLightColor() const
{
    std::shared_ptr<SceneNode> rootSceneNode = m_sceneHierarchy->GetRootSceneNode();
    if (rootSceneNode != nullptr)
    {
        std::shared_ptr<OpenGLSettingsComponent> openGLSettingsComponent = rootSceneNode->GetObject().
            GetFirstComponentOfType<OpenGLSettingsComponent>();
        if (openGLSettingsComponent != nullptr)
        {
            m_defaultShader->SetUniform<glm::vec3>("ambientColor", openGLSettingsComponent->GetAmbientLight());
        }
    }
}

void OpenGLRenderer::ProcessObject(const Object& object, std::shared_ptr<OpenGLShader> activeShader) const
{
    if (std::shared_ptr<PrimitiveComponent> primitiveComponent = object.GetFirstComponentOfType<PrimitiveComponent>())
    {
        if (std::shared_ptr<TransformComponent> transformComponent = object.GetFirstComponentOfType<
            TransformComponent>())
        {
            if (std::shared_ptr<MaterialComponent> materialComponent = object.GetFirstComponentOfType<
                MaterialComponent>())
            {
                DrawMesh(*primitiveComponent, *transformComponent, *materialComponent, activeShader);
            }
        }
    }
}

void OpenGLRenderer::DrawMesh(const PrimitiveComponent& primitiveComponent,
                              const TransformComponent& transformComponent,
                              const MaterialComponent& materialComponent,
                              std::shared_ptr<OpenGLShader> activeShader) const
{
    glm::mat4 cumulativeModelMatrix = transformComponent.GetCumulativeModelMatrix();
    activeShader->SetUniform<glm::mat4>("modelMatrix", false, cumulativeModelMatrix);

    if (activeShader == m_defaultShader)
    {
        // Set default shader-exclusive uniforms
        glm::mat3 inverseTransposeModelMatrix = transpose(inverse(glm::mat3(cumulativeModelMatrix)));
        activeShader->SetUniform<glm::mat3>("inverseTransposeModelMatrix", false, inverseTransposeModelMatrix);

        // Color
        activeShader->SetUniform<bool>("useMaterialTexture", materialComponent.GetUseColorTexture());
        if (materialComponent.GetUseColorTexture())
        {
            GLuint64 textureHandle = m_openGLTextureCache->GetTexture(materialComponent.GetMaterialTexture())->
                                                           GetTextureHandle();
            activeShader->SetUniform<GLuint64>("materialTexture", textureHandle);
        }
        else
        {
            activeShader->SetUniform<glm::vec4>("materialColor", materialComponent.GetMaterialColor());
        }

        // Metallic
        activeShader->SetUniform<bool>("useMetallicMap", materialComponent.GetUseMetallicMap());
        if (materialComponent.GetUseMetallicMap())
        {
            GLuint64 textureHandle = m_openGLTextureCache->GetTexture(materialComponent.GetMetallicMap())->
                                                           GetTextureHandle();
            activeShader->SetUniform<GLuint64>("metallicMap", textureHandle);
        }
        else
        {
            activeShader->SetUniform<float>("metallic", materialComponent.GetMetallic());
        }

        // Roughness
        activeShader->SetUniform<bool>("useRoughnessMap", materialComponent.GetUseRoughnessMap());
        if (materialComponent.GetUseRoughnessMap())
        {
            GLuint64 textureHandle = m_openGLTextureCache->GetTexture(materialComponent.GetRoughnessMap())->
                                                           GetTextureHandle();
            activeShader->SetUniform<GLuint64>("roughnessMap", textureHandle);
        }
        else
        {
            activeShader->SetUniform<float>("roughness", materialComponent.GetRoughness());
        }
    }
}

void OpenGLRenderer::OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const
{
    m_openGLLightContainer->TryAddLight(newSceneNode);
}
