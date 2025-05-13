#include "OpenGLBufferManager.h"

#include "../../../../OpenGLHelpers/Shader/OpenGLShaderBlock.h"
#include "../../../../Scene/Object.h"
#include "../../../../Scene/SceneHierarchy.h"
#include "../../../../Scene/SceneNode/SceneNode.h"
#include "../OpenGLBuffer.h"
#include "../../../../OpenGLHelpers/Lights/OpenGLLight.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"
#include "../../OpenGLHelpers/OpenGLTexture.h"
#include "../../Scene/Components/ComponentIncludes.h"

OpenGLBufferManager::OpenGLBufferManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                                         std::shared_ptr<OpenGLTextureCache> textureCache,
                                         std::shared_ptr<OpenGLShaderBlock> modelMatrixBlock,
                                         std::shared_ptr<OpenGLShaderBlock> inverseTransposeModelMatrixBlock,
                                         std::shared_ptr<OpenGLShaderBlock> materialBlock,
                                         std::shared_ptr<OpenGLShaderBlock> lightBlock):
    m_sceneHierarchy(sceneHierarchy),
    m_textureCache(textureCache),
    m_modelMatrixBlock(modelMatrixBlock),
    m_inverseTransposeModelMatrixBlock(inverseTransposeModelMatrixBlock),
    m_materialBlock(materialBlock),
    m_lightBlock(lightBlock)
{
    m_lightBuffer = std::make_shared<OpenGLBuffer>(m_lightBlock->GetDataSize(), GL_UNIFORM_BUFFER,
                                                   GL_DYNAMIC_DRAW);
    m_sceneHierarchy->SubscribeToSceneNodeAdded([this](const std::shared_ptr<SceneNode>&)
    {
        RegenerateBuffers();
    });

    m_sceneHierarchy->SubscribeToSceneNodeRemoved([this](const std::shared_ptr<SceneNode>&)
    {
        RegenerateBuffers();
    });
}

const std::shared_ptr<OpenGLBuffer>& OpenGLBufferManager::GetModelMatrixBuffer() const
{
    return m_modelMatrixBuffer;
}

const std::shared_ptr<OpenGLBuffer>& OpenGLBufferManager::GetInverseTransposeModelMatrixBuffer() const
{
    return m_inverseTransposeModelMatrixBuffer;
}

const std::shared_ptr<OpenGLBuffer>& OpenGLBufferManager::GetMaterialBuffer() const
{
    return m_materialBuffer;
}

const std::shared_ptr<OpenGLBuffer>& OpenGLBufferManager::GetLightBuffer() const
{
    return m_lightBuffer;
}

void OpenGLBufferManager::RegenerateBuffers()
{
    int drawableCount = 0;
    m_sceneHierarchy->BreadthFirstProcessAllSceneNodes(
        [this, &drawableCount](std::shared_ptr<SceneNode> sceneNode)
        {
            if (sceneNode->ContainsComponent<MaterialComponent>() && sceneNode->ContainsComponent<PrimitiveComponent>()
                && sceneNode->ContainsComponent<TransformComponent>())
            {
                drawableCount++;
            }
        });

    m_modelMatrixBuffer = std::make_shared<OpenGLBuffer>(
        drawableCount * m_modelMatrixBlock->operator()("modelMatrices").GetArrayStride(), GL_SHADER_STORAGE_BUFFER,
        GL_DYNAMIC_DRAW);
    m_inverseTransposeModelMatrixBuffer = std::make_shared<OpenGLBuffer>(
        drawableCount * m_inverseTransposeModelMatrixBlock->operator()("inverseTransposeModelMatrices").
                                                            GetArrayStride(), GL_SHADER_STORAGE_BUFFER,
        GL_DYNAMIC_DRAW);
    m_materialBuffer = std::make_shared<OpenGLBuffer>(
        drawableCount * m_materialBlock->operator()("materials").GetArrayStride(), GL_SHADER_STORAGE_BUFFER,
        GL_DYNAMIC_DRAW);

    int drawIndex = 0;
    int lightIndex = 0;
    m_sceneHierarchy->BreadthFirstProcessAllSceneNodes(
        [this, &drawIndex, &lightIndex](std::shared_ptr<SceneNode> sceneNode)
        {
            if (sceneNode->ContainsComponent<TransformComponent>() && sceneNode->ContainsComponent<PrimitiveComponent>()
                && sceneNode->ContainsComponent<MaterialComponent>())
            {
                SubscribeToPrimitiveSceneNode(sceneNode, drawIndex);
                drawIndex++;
            }
            if (sceneNode->ContainsComponent<TransformComponent>() && (sceneNode->ContainsComponent<PointLightComponent>()
                || sceneNode->ContainsComponent<SpotLightComponent>() || sceneNode->ContainsComponent<
                    DirectionalLightComponent>()))
            {
                SubscribeToLightSceneNode(sceneNode, lightIndex);
                lightIndex++;
            }
        });
    m_lightBuffer->SetSubData(m_lightBlock->operator()("lightCount").GetCumulativeOffset(), lightIndex);
}

void OpenGLBufferManager::SubscribeToPrimitiveSceneNode(const std::shared_ptr<SceneNode>& sceneNode,
                                                        int drawIndex) const
{
    std::shared_ptr<TransformComponent> transformComponent = sceneNode->GetObject().GetFirstComponentOfType<
        TransformComponent>();
    std::shared_ptr<PrimitiveComponent> primitiveComponent = sceneNode->GetObject().GetFirstComponentOfType<
        PrimitiveComponent>();
    std::shared_ptr<MaterialComponent> materialComponent = sceneNode->GetObject().GetFirstComponentOfType<
        MaterialComponent>();

    // Transform Buffers
    transformComponent->GetCumulativeModelMatrixDataBinding().TryUnsubscribe(this);
    transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(
        this, [this, drawIndex](const glm::mat4& modelMatrix, glm::mat4)
        {
            m_modelMatrixBuffer->SetSubData(
                m_modelMatrixBlock->operator()("modelMatrices")[drawIndex].
                GetCumulativeOffset(), modelMatrix);

            glm::mat4 invTranspose = glm::transpose(glm::inverse(modelMatrix));
            m_inverseTransposeModelMatrixBuffer->SetSubData(m_inverseTransposeModelMatrixBlock->operator(
                                                            )("inverseTransposeModelMatrices")[drawIndex].
                                                            GetCumulativeOffset(), invTranspose);
        },
        true);

    // Material Buffer
    materialComponent->GetUseColorTextureDataBinding().TryUnsubscribe(this);
    materialComponent->GetUseColorTextureDataBinding().Subscribe(
        this, [this, drawIndex](const bool& useColorTexture, bool)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            m_materialBuffer->SetSubData<int>(
                materialBlockArray[drawIndex]("useMaterialTexture").GetCumulativeOffset(),
                useColorTexture);
        },
        true);
    materialComponent->GetMaterialColorDataBinding().TryUnsubscribe(this);
    materialComponent->GetMaterialColorDataBinding().Subscribe(
        this, [this, drawIndex](const glm::vec4& color, glm::vec4)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("materialColor").GetCumulativeOffset(),
                                         color);
        },
        true);
    materialComponent->GetMaterialTextureDataBinding().TryUnsubscribe(this);
    materialComponent->GetMaterialTextureDataBinding().Subscribe(
        this, [this, drawIndex](const std::string& materialFilePath, std::string)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            if (materialFilePath.empty())
            {
                return;
            }
            std::shared_ptr<OpenGLTexture> materialTexture = m_textureCache->GetTexture(
                materialFilePath);
            m_materialBuffer->SetSubData(
                materialBlockArray[drawIndex]("materialTexture").GetCumulativeOffset(),
                materialTexture->GetTextureHandle());
        },
        true);
    materialComponent->GetUseRoughnessMapDataBinding().TryUnsubscribe(this);
    materialComponent->GetUseRoughnessMapDataBinding().Subscribe(
        this, [this, drawIndex](const bool& useRoughnessMap, bool)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            m_materialBuffer->SetSubData<int>(
                materialBlockArray[drawIndex]("useRoughnessMap").GetCumulativeOffset(),
                useRoughnessMap);
        },
        true);
    materialComponent->GetRoughnessDataBinding().TryUnsubscribe(this);
    materialComponent->GetRoughnessDataBinding().Subscribe(
        this, [this, drawIndex](const float& roughness, float)
        {
            BlockMember materialBlockArray = m_materialBlock->operator
                ()("materials");
            m_materialBuffer->SetSubData(
                materialBlockArray[drawIndex]("roughness").
                GetCumulativeOffset(), roughness);
        },
        true);
    materialComponent->GetRoughnessMapDataBinding().TryUnsubscribe(this);
    materialComponent->GetRoughnessMapDataBinding().Subscribe(
        this, [this, drawIndex](const std::string& roughnessMapFilePath, std::string)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            if (roughnessMapFilePath.empty())
            {
                return;
            }
            std::shared_ptr<OpenGLTexture> roughnessMap = m_textureCache->GetTexture(
                roughnessMapFilePath);
            m_materialBuffer->SetSubData(
                materialBlockArray[drawIndex]("roughnessMap").GetCumulativeOffset(),
                roughnessMap->GetTextureHandle());
        },
        true);
    materialComponent->GetUseMetallicMapDataBinding().TryUnsubscribe(this);
    materialComponent->GetUseMetallicMapDataBinding().Subscribe(
        this, [this, drawIndex](const bool& useMetallicMap, bool)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            m_materialBuffer->SetSubData<int>(materialBlockArray[drawIndex]("useMetallicMap").GetCumulativeOffset(),
                                              useMetallicMap);
        },
        true);
    materialComponent->GetMetallicDataBinding().TryUnsubscribe(this);
    materialComponent->GetMetallicDataBinding().Subscribe(this, [this, drawIndex](const float& metallic, float)
                                                          {
                                                              BlockMember materialBlockArray =
                                                                  m_materialBlock->operator(
                                                                  )("materials");
                                                              m_materialBuffer->SetSubData(
                                                                  materialBlockArray[drawIndex]("metallic").
                                                                  GetCumulativeOffset(), metallic);
                                                          },
                                                          true);
    materialComponent->GetMetallicMapDataBinding().TryUnsubscribe(this);
    materialComponent->GetMetallicMapDataBinding().Subscribe(
        this, [this, drawIndex](const std::string& metallicMapFilePath, std::string)
        {
            BlockMember materialBlockArray = m_materialBlock->operator(
            )("materials");
            if (metallicMapFilePath.empty())
            {
                return;
            }
            std::shared_ptr<OpenGLTexture> metallicMap = m_textureCache->GetTexture(metallicMapFilePath);
            m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("metallicMap").GetCumulativeOffset(),
                                         metallicMap->
                                         GetTextureHandle());
        },
        true);
}

void OpenGLBufferManager::SubscribeToLightSceneNode(const std::shared_ptr<SceneNode>& sceneNode, int lightIndex) const
{
    std::shared_ptr<TransformComponent> transformComponent = sceneNode->GetObject().GetFirstComponentOfType<
        TransformComponent>();
    std::shared_ptr<PointLightComponent> pointLightComponent = sceneNode->GetObject().GetFirstComponentOfType<
        PointLightComponent>();
    std::shared_ptr<DirectionalLightComponent> directionalLightComponent = sceneNode->GetObject().
        GetFirstComponentOfType<DirectionalLightComponent>();
    std::shared_ptr<SpotLightComponent> spotLightComponent = sceneNode->GetObject().GetFirstComponentOfType<
        SpotLightComponent>();

    m_lightBuffer->SetSubData<int>(m_lightBlock->operator()("lights")[lightIndex]("hasShadowMap").GetCumulativeOffset(),
                                   false);

    if (pointLightComponent)
    {
        m_lightBuffer->SetSubData<int>(m_lightBlock->operator()("lights")[lightIndex]("type").GetCumulativeOffset(),
                                       Point);
        transformComponent->GetCumulativeModelMatrixDataBinding().TryUnsubscribe(this);
        transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(
            this, [this, transformComponent, lightIndex](const glm::mat4&, glm::mat4)
            {
                glm::vec3 position = transformComponent->GetWorldSpacePosition();
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionX").GetCumulativeOffset(), position.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionY").GetCumulativeOffset(), position.y);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionZ").GetCumulativeOffset(), position.z);
            }, true);
        pointLightComponent->GetFalloffDataBinding().TryUnsubscribe(this);
        pointLightComponent->GetFalloffDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& falloff, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("falloffA").GetCumulativeOffset(), falloff.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("falloffB").GetCumulativeOffset(), falloff.y);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("falloffC").GetCumulativeOffset(), falloff.z);
            }, true);
        pointLightComponent->GetLightColorDataBinding().TryUnsubscribe(this);
        pointLightComponent->GetLightColorDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& color, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorR").GetCumulativeOffset(), color.r);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorG").GetCumulativeOffset(), color.g);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorB").GetCumulativeOffset(), color.b);
            }, true);
    }
    if (spotLightComponent)
    {
        m_lightBuffer->SetSubData<int>(m_lightBlock->operator()("lights")[lightIndex]("type").GetCumulativeOffset(),
                                       Spot);
        transformComponent->GetCumulativeModelMatrixDataBinding().TryUnsubscribe(this);
        transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(
            this, [this, transformComponent, lightIndex](const glm::mat4&, glm::mat4)
            {
                glm::vec3 position = transformComponent->GetWorldSpacePosition();
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionX").GetCumulativeOffset(), position.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionY").GetCumulativeOffset(), position.y);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("positionZ").GetCumulativeOffset(), position.z);
            }, true);
        transformComponent->GetWorldSpaceXUnitVectorDataBinding().TryUnsubscribe(this);
        transformComponent->GetWorldSpaceXUnitVectorDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& direction, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionX").GetCumulativeOffset(), direction.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionY").GetCumulativeOffset(), direction.y);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionZ").GetCumulativeOffset(), direction.z);
            }, true);
        spotLightComponent->GetLightFalloffAnglesDataBinding().TryUnsubscribe(this);
        spotLightComponent->GetLightFalloffAnglesDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec2& falloff, glm::vec2)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("falloffA").GetCumulativeOffset(), falloff.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("falloffB").GetCumulativeOffset(), falloff.y);
            }, true);
        spotLightComponent->GetLightColorDataBinding().TryUnsubscribe(this);
        spotLightComponent->GetLightColorDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& color, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorR").GetCumulativeOffset(), color.r);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorG").GetCumulativeOffset(), color.g);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorB").GetCumulativeOffset(), color.b);
            }, true);
    }
    if (directionalLightComponent)
    {
        m_lightBuffer->SetSubData<int>(m_lightBlock->operator()("lights")[lightIndex]("type").GetCumulativeOffset(),
                                       Directional);
        transformComponent->GetWorldSpaceXUnitVectorDataBinding().TryUnsubscribe(this);
        transformComponent->GetWorldSpaceXUnitVectorDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& direction, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionX").GetCumulativeOffset(), direction.x);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionY").GetCumulativeOffset(), direction.y);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("directionZ").GetCumulativeOffset(), direction.z);
            }, true);
        directionalLightComponent->GetLightColorDataBinding().TryUnsubscribe(this);
        directionalLightComponent->GetLightColorDataBinding().Subscribe(
            this, [this, lightIndex](const glm::vec3& color, glm::vec3)
            {
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorR").GetCumulativeOffset(), color.r);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorG").GetCumulativeOffset(), color.g);
                m_lightBuffer->SetSubData<float>(
                    m_lightBlock->operator()("lights")[lightIndex]("colorB").GetCumulativeOffset(), color.b);
            }, true);
    }
}
