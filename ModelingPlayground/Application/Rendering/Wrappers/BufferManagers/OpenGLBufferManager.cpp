#include "OpenGLBufferManager.h"

#include "../../../../OpenGLHelpers/Shader/OpenGLShaderStorageBlock.h"
#include "../../../../Scene/Object.h"
#include "../../../../Scene/SceneHierarchy.h"
#include "../../../../Scene/SceneNode/SceneNode.h"
#include "../OpenGLBuffer.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"
#include "../../OpenGLHelpers/OpenGLTexture.h"
#include "../../Scene/Components/ComponentIncludes.h"

OpenGLBufferManager::OpenGLBufferManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                                         std::shared_ptr<OpenGLTextureCache> textureCache,
                                         std::shared_ptr<OpenGLShaderStorageBlock> modelMatrixBlock,
                                         std::shared_ptr<OpenGLShaderStorageBlock> inverseTransposeModelMatrixBlock,
                                         std::shared_ptr<OpenGLShaderStorageBlock> materialBlock):
    m_sceneHierarchy(sceneHierarchy),
    m_textureCache(textureCache),
    m_modelMatrixBlock(modelMatrixBlock),
    m_inverseTransposeModelMatrixBlock(inverseTransposeModelMatrixBlock),
    m_materialBlock(materialBlock)
{
    m_sceneHierarchy->SubscribeToSceneNodeAdded([this](const std::shared_ptr<SceneNode>&)
    {
        int drawableCount = 0;
        m_sceneHierarchy->BreadthFirstProcessAllSceneNodes([this, &drawableCount](std::shared_ptr<SceneNode> sceneNode)
        {
            if (!sceneNode->GetObject().GetFirstComponentOfType<MaterialComponent>())
            {
                return;
            }
            if (!sceneNode->GetObject().GetFirstComponentOfType<PrimitiveComponent>())
            {
                return;
            }
            if (!sceneNode->GetObject().GetFirstComponentOfType<TransformComponent>())
            {
                return;
            }
            drawableCount++;
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
        m_sceneHierarchy->BreadthFirstProcessAllSceneNodes([this, &drawIndex](std::shared_ptr<SceneNode> sceneNode)
        {
            std::shared_ptr<TransformComponent> transformComponent = sceneNode->GetObject().GetFirstComponentOfType<
                TransformComponent>();
            std::shared_ptr<PrimitiveComponent> primitiveComponent = sceneNode->GetObject().GetFirstComponentOfType<
                PrimitiveComponent>();
            std::shared_ptr<MaterialComponent> materialComponent = sceneNode->GetObject().GetFirstComponentOfType<
                MaterialComponent>();
            if (!transformComponent || !primitiveComponent || !materialComponent)
            {
                return;
            }
            // Transform Buffers
            transformComponent->GetCumulativeModelMatrixDataBinding().TryUnsubscribe(this);
            transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(
                this, [this, drawIndex, &transformComponent](const glm::mat4& modelMatrix, glm::mat4)
                {
                    m_modelMatrixBuffer->SetSubData(
                        m_modelMatrixBlock->operator()("modelMatrices")[drawIndex].
                        GetCumulativeOffset(), modelMatrix);

                    glm::mat3 invTranspose = glm::transpose(glm::inverse(modelMatrix));
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("materialColor").GetCumulativeOffset(),
                                                 color);
                },
                true);
            materialComponent->GetMaterialTextureDataBinding().TryUnsubscribe(this);
            materialComponent->GetMaterialTextureDataBinding().Subscribe(
                this, [this, drawIndex](const std::string& materialFilePath, std::string)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData<int>(materialBlockArray[drawIndex]("useRoughnessMap").GetCumulativeOffset(),
                                                 useRoughnessMap);
                },
                true);
            materialComponent->GetRoughnessDataBinding().TryUnsubscribe(this);
            materialComponent->GetRoughnessDataBinding().Subscribe(
                this, [this, drawIndex](const float& roughness, float)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData<int>(materialBlockArray[drawIndex]("useMetallicMap").GetCumulativeOffset(),
                                                 useMetallicMap);
                },
                true);
            materialComponent->GetMetallicDataBinding().TryUnsubscribe(this);
            materialComponent->GetMetallicDataBinding().Subscribe(this, [this, drawIndex](const float& metallic, float)
                                                                  {
                                                                      BufferProperty materialBlockArray =
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
                    BufferProperty materialBlockArray = m_materialBlock->operator(
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

            drawIndex++;
        });
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
