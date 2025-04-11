#include "ModelLoader.h"

#include <iostream>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../OpenGLHelpers/OpenGLRenderer.h"
#include "../Scene/Object.h"
#include "../Scene/Components/PrimitiveComponent.h"
#include "../Scene/SceneNode/SceneNode.h"
#include "../Scene/SceneNode/SceneNodeGenerator.h"

void ModelLoader::LoadModel(const std::shared_ptr<SceneNode>& parentSceneNode, const std::string& filePath,
                            const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
                            const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		filePath, aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ModelLoader: ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	// Process nodes
	std::stack<std::tuple<aiNode*, std::shared_ptr<SceneNode>, std::string>> nodeStack;
	nodeStack.push({scene->mRootNode, parentSceneNode, filePath});
	while (!nodeStack.empty())
	{
		aiNode* nodeToProcess = std::get<0>(nodeStack.top());
		std::shared_ptr<SceneNode> parent = std::get<1>(nodeStack.top());
		std::string primitiveNameBase = std::get<2>(nodeStack.top());
		nodeStack.pop();

		ProcessNode(nodeToProcess, scene, parent, primitiveNameBase, openGLRenderer, sceneNodeGenerator);

		std::shared_ptr<SceneNode> newParent = parent->HasChildren() ? parent->GetChildren().front() : parent;
		for (unsigned int i = 0; i < nodeToProcess->mNumChildren; i++)
		{
			nodeStack.push({
				nodeToProcess->mChildren[i], newParent,
				primitiveNameBase + "|0|" + std::to_string(i)
			});
		}
	}

	// Process lights
	ProcessLights(scene, parentSceneNode, sceneNodeGenerator);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<SceneNode>& parentSceneNode,
                              const std::string& primitiveNameBase,
                              const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
                              const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator)
{
	// Create transform node
	aiVector3t<float> position;
	aiVector3t<float> rotation;
	aiVector3t<float> scale;
	node->mTransformation.Decompose(scale, rotation, position);

	AddPrimitiveNodes(node, scene, glm::vec3(position.x, position.y, position.z),
	                  degrees(glm::vec3(rotation.x, rotation.y, rotation.z)), glm::vec3(scale.x, scale.y, scale.z),
	                  parentSceneNode, primitiveNameBase, openGLRenderer, sceneNodeGenerator);
}

void ModelLoader::AddPrimitiveNodes(aiNode* node, const aiScene* scene, glm::vec3 position, glm::vec3 rotation,
                                    glm::vec3 scale,
                                    const std::shared_ptr<SceneNode>& parentSceneNode,
                                    const std::string& primitiveNameBase,
                                    const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
                                    const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		std::vector<float> vertices;
		std::vector<int> indices;
		bool hasTexCoords = false;

		ProcessMesh(mesh, vertices, indices, hasTexCoords);

		std::vector<VertexAttribute> vertexAttributes = {VertexAttribute::Position, VertexAttribute::Normal};
		if (hasTexCoords)
		{
			vertexAttributes.push_back(VertexAttribute::UV);
		}

		auto openGLPrimitive = std::make_shared<OpenGLPrimitive>(vertices, indices, vertexAttributes);
		std::string primitiveName = primitiveNameBase + "|" + std::to_string(i);
		openGLRenderer->GetOpenGLPrimitiveManager()->AddPrimitive(primitiveName,
		                                                          openGLPrimitive);

		std::shared_ptr<SceneNode> childSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
			SceneNodeType::Primitive, parentSceneNode);

		// Set up primitive component
		std::shared_ptr<PrimitiveComponent> primitiveComponent = childSceneNode->GetObject().GetFirstComponentOfType
			<PrimitiveComponent>();
		primitiveComponent->SetPrimitiveName(primitiveName);

		// Set up transform component
		std::shared_ptr<TransformComponent> transformComponent = childSceneNode->GetObject().GetFirstComponentOfType
			<TransformComponent>();
		transformComponent->GetPositionDataBinding().SetAndNotify(glm::vec3(position.x, position.y, position.z));
		transformComponent->GetRotationDataBinding().SetAndNotify(
			degrees(glm::vec3(rotation.x, rotation.y, rotation.z)));
		transformComponent->GetScaleDataBinding().SetAndNotify(glm::vec3(scale.x, scale.y, scale.z));
	}
}

void ModelLoader::ProcessMesh(aiMesh* mesh, std::vector<float>& vertices, std::vector<int>& indices, bool& hasTexCoords)
{
	// Vertices
	hasTexCoords = mesh->mTextureCoords[0] != nullptr;
	unsigned int vertexSize = hasTexCoords ? 8 : 6;
	vertices.resize(mesh->mNumVertices * vertexSize);

	for (unsigned int j = 0; j < mesh->mNumVertices; j++)
	{
		vertices[vertexSize * j] = mesh->mVertices[j].x;
		vertices[vertexSize * j + 1] = mesh->mVertices[j].y;
		vertices[vertexSize * j + 2] = mesh->mVertices[j].z;

		vertices[vertexSize * j + 3] = mesh->mNormals[j].x;
		vertices[vertexSize * j + 4] = mesh->mNormals[j].y;
		vertices[vertexSize * j + 5] = mesh->mNormals[j].z;

		if (hasTexCoords)
		{
			vertices[vertexSize * j + 6] = mesh->mTextureCoords[0][j].x;
			vertices[vertexSize * j + 7] = mesh->mTextureCoords[0][j].y;
		}
	}

	// Indices
	indices.resize(mesh->mNumFaces * 3);
	for (unsigned int j = 0; j < mesh->mNumFaces; j++)
	{
		aiFace face = mesh->mFaces[j];
		if (face.mNumIndices != 3)
		{
			std::cerr << "ModelLoader|LoadModel: Error! Model is not triangulated!\n";
			return;
		}

		indices[3 * j] = face.mIndices[0];
		indices[3 * j + 1] = face.mIndices[1];
		indices[3 * j + 2] = face.mIndices[2];
	}
}

void ModelLoader::ProcessLights(const aiScene* scene, const std::shared_ptr<SceneNode>& parentSceneNode,
                                const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator)
{
	for (unsigned int i = 0; i < scene->mNumLights; i++)
	{
		aiLight* light = scene->mLights[i];

		auto lightPosition = glm::vec3(light->mPosition.x, light->mPosition.y, light->mPosition.z);
		auto lightDirection = glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z);

		switch (light->mType)
		{
		case aiLightSource_POINT:
			{
				std::shared_ptr<SceneNode> pointLightSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
					SceneNodeType::PointLight, parentSceneNode);
				pointLightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()->GetPositionDataBinding()
				                   .SetAndNotify(lightPosition);
			}
			break;
		case aiLightSource_DIRECTIONAL:
			{
				std::shared_ptr<SceneNode> directionalLightSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
					SceneNodeType::DirectionalLight, parentSceneNode);
				directionalLightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()->
				                           GetLocalXUnitVectorDataBinding()
				                           .SetAndNotify(lightDirection);
			}
			break;
		case aiLightSource_SPOT:
			{
				std::shared_ptr<SceneNode> spotLightSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
					SceneNodeType::SpotLight, parentSceneNode);
				spotLightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()->GetPositionDataBinding()
				                  .SetAndNotify(lightPosition);
				spotLightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()->
				                    GetLocalXUnitVectorDataBinding()
				                    .SetAndNotify(lightDirection);
			}
			break;
		default:
			std::cout << "ModelLoader|ProcessLights: Model contains unsupported light type, skipping\n";
			break;
		}
	}
}
