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
            /*std::shared_ptr<PrimitiveComponent> primitiveComponent = sceneNode->GetObject().GetFirstComponentOfType<
                PrimitiveComponent>();*/
            std::shared_ptr<MaterialComponent> materialComponent = sceneNode->GetObject().GetFirstComponentOfType<
                MaterialComponent>();
            if (!transformComponent /*|| !primitiveComponent*/ || !materialComponent)
            {
                return;
            }
            // Transform Buffers
            transformComponent->GetCumulativeModelMatrixDataBinding().Unsubscribe(this);
            transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(
                this, [this, drawIndex, &transformComponent](const glm::mat4& modelMatrix, glm::mat4)
                {
                    m_modelMatrixBuffer->SetSubData(
                        m_modelMatrixBlock->operator()("modelMatrices")[drawIndex].
                        GetCumulativeOffset(), modelMatrix);
                    m_inverseTransposeModelMatrixBuffer->SetSubData(m_inverseTransposeModelMatrixBlock->operator(
                                                                    )("inverseTransposeModelMatrices")[drawIndex].
                                                                    GetCumulativeOffset(),
                                                                    transformComponent->
                                                                    GetInverseTransposeCumulativeModelMatrix());
                },
                true);

            // Material Buffer
            materialComponent->GetUseColorTextureDataBinding().Subscribe(
                this, [this, drawIndex](const bool& useColorTexture, bool)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData(
                        materialBlockArray[drawIndex]("useMaterialTexture").GetCumulativeOffset(),
                        useColorTexture);
                },
                true);
            materialComponent->GetMaterialColorDataBinding().Subscribe(
                this, [this, drawIndex](const glm::vec3& color, glm::vec3)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("materialColor").GetCumulativeOffset(),
                                                 color);
                },
                true);
            materialComponent->GetMaterialTextureDataBinding().Subscribe(
                this, [this, drawIndex](const std::string& materialFilePath, std::string)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    std::shared_ptr<OpenGLTexture> materialTexture = m_textureCache->GetTexture(
                        materialFilePath);
                    if (materialTexture)
                    {
                        m_materialBuffer->SetSubData(
                            materialBlockArray[drawIndex]("materialTexture").GetCumulativeOffset(),
                            materialTexture->GetTextureHandle());
                    }
                },
                true);

            materialComponent->GetUseRoughnessMapDataBinding().Subscribe(
                this, [this, drawIndex](const bool& useRoughnessMap, bool)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("useRoughnessMap").GetCumulativeOffset(),
                                                 useRoughnessMap);
                },
                true);
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
            materialComponent->GetRoughnessMapDataBinding().Subscribe(
                this, [this, drawIndex](const std::string& roughnessMapFilePath, std::string)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    std::shared_ptr<OpenGLTexture> roughnessMap = m_textureCache->GetTexture(
                        roughnessMapFilePath);
                    if (roughnessMap)
                    {
                        m_materialBuffer->SetSubData(
                            materialBlockArray[drawIndex]("roughnessMap").GetCumulativeOffset(),
                            roughnessMap->GetTextureHandle());
                    }
                },
                true);

            materialComponent->GetUseMetallicMapDataBinding().Subscribe(
                this, [this, drawIndex](const bool& useMetallicMap, bool)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("useMetallicMap").GetCumulativeOffset(),
                                                 useMetallicMap);
                },
                true);
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
            materialComponent->GetMetallicMapDataBinding().Subscribe(
                this, [this, drawIndex](const std::string& metallicMapFilePath, std::string)
                {
                    BufferProperty materialBlockArray = m_materialBlock->operator(
                    )("materials");
                    std::shared_ptr<OpenGLTexture> metallicMap = m_textureCache->GetTexture(metallicMapFilePath);
                    if (metallicMap)
                    {
                        m_materialBuffer->SetSubData(materialBlockArray[drawIndex]("metallicMap").GetCumulativeOffset(),
                                                     metallicMap->
                                                     GetTextureHandle());
                    }
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
