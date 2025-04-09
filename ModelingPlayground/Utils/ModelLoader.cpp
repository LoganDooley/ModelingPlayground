#include "ModelLoader.h"

#include <iostream>
#include <ostream>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>>
#include <assimp/scene.h>

#include "../OpenGLHelpers/OpenGLRenderer.h"
#include "../Scene/Object.h"
#include "../Scene/Components/PrimitiveComponent.h"
#include "../Scene/SceneNode/SceneNode.h"
#include "../Scene/SceneNode/SceneNodeGenerator.h"

void ModelLoader::LoadModel(const std::shared_ptr<SceneNode>& parentSceneNode, const std::string& filePath,
                            const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
                            std::shared_ptr<SceneNodeGenerator> sceneNodeGenerator)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		filePath, aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ModelLoader: ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	// pair: aiNode, parent scene node
	std::stack<std::tuple<aiNode*, std::shared_ptr<SceneNode>, std::string>> nodeStack;
	nodeStack.push({scene->mRootNode, parentSceneNode, filePath});
	while (!nodeStack.empty())
	{
		aiNode* nodeToProcess = std::get<0>(nodeStack.top());
		std::shared_ptr<SceneNode> parentSceneNode = std::get<1>(nodeStack.top());
		std::string primitiveNameBase = std::get<2>(nodeStack.top());
		nodeStack.pop();

		// Create transform node
		aiVector3t<float> position;
		aiVector3t<float> rotation;
		aiVector3t<float> scale;
		nodeToProcess->mTransformation.Decompose(scale, rotation, position);
		std::shared_ptr<SceneNode> transformSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
			SceneNodeType::Transform, parentSceneNode);
		std::shared_ptr<TransformComponent> transformComponent = transformSceneNode->GetObject().GetFirstComponentOfType
			<TransformComponent>();
		transformComponent->GetPositionDataBinding().SetAndNotify(glm::vec3(position.x, position.y, position.z));
		transformComponent->GetRotationDataBinding().SetAndNotify(
			degrees(glm::vec3(rotation.x, rotation.y, rotation.z)));
		transformComponent->GetScaleDataBinding().SetAndNotify(glm::vec3(scale.x, scale.y, scale.z));

		for (unsigned int i = 0; i < nodeToProcess->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[nodeToProcess->mMeshes[i]];

			// Vertices
			bool hasTextureCoords = mesh->mTextureCoords[0] != nullptr;
			hasTextureCoords = false;
			std::vector<float> vertices;
			unsigned int vertexSize = hasTextureCoords ? 8 : 6;
			if (hasTextureCoords)
			{
				vertices.resize(mesh->mNumVertices * 8);
			}
			else
			{
				vertices.resize(mesh->mNumVertices * 6);
			}

			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{
				vertices[vertexSize * j] = mesh->mVertices[j].x;
				vertices[vertexSize * j + 1] = mesh->mVertices[j].y;
				vertices[vertexSize * j + 2] = mesh->mVertices[j].z;

				vertices[vertexSize * j + 3] = mesh->mNormals[j].x;
				vertices[vertexSize * j + 4] = mesh->mNormals[j].y;
				vertices[vertexSize * j + 5] = mesh->mNormals[j].z;

				if (hasTextureCoords)
				{
					vertices[vertexSize * j + 6] = mesh->mTextureCoords[0][j].x;
					vertices[vertexSize * j + 7] = mesh->mTextureCoords[0][j].y;
				}
			}

			// Indices
			std::vector<int> indices(mesh->mNumFaces * 3);
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

			auto openGLPrimitive = std::make_shared<OpenGLPrimitive>(vertices, mesh->mNumVertices, hasTextureCoords,
			                                                         indices);
			std::string primitiveName = primitiveNameBase + "|" + std::to_string(i);
			openGLRenderer->GetOpenGLPrimitiveManager()->AddPrimitive(primitiveName,
			                                                          openGLPrimitive);

			std::shared_ptr<SceneNode> childSceneNode = sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
				SceneNodeType::Primitive, transformSceneNode);

			std::shared_ptr<PrimitiveComponent> primitiveComponent = childSceneNode->GetObject().GetFirstComponentOfType
				<PrimitiveComponent>();

			primitiveComponent->SetPrimitiveName(primitiveName);
		}

		for (unsigned int i = 0; i < nodeToProcess->mNumChildren; i++)
		{
			nodeStack.push({nodeToProcess->mChildren[i], transformSceneNode, primitiveNameBase + "|0|" + std::to_string(i)});
		}
	}
}
