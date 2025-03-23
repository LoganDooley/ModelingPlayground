#include "OpenGLPrimitiveManager.h"

#include <utility>
#include <vector>
#include <glm/vec3.hpp>

#include "../../Utils/PrimitiveGenerator.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <ranges>

#include "../Libraries/tinyobjloader/tiny_obj_loader.h"

OpenGLPrimitiveManager::OpenGLPrimitiveManager():
	m_primitives({})
{
    
}

void OpenGLPrimitiveManager::GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution)
{
	// Initialize Triangle
    float triangleData[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

	std::vector<OpenGLVertex> triangleVertices = {
		OpenGLVertex(glm::vec3( -0.5f, -0.5f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0, 0), true),
		OpenGLVertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1, 0), true),
		OpenGLVertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5, 1), true)
	};

	m_primitives["Triangle"] = std::make_shared<OpenGLPrimitive>(triangleVertices);


    std::pair<std::vector<OpenGLVertex>, std::vector<int>> sphereData = PrimitiveGenerator::GenerateSphere(sphereLatitudinalResolution, sphereLongitudinalResolution);
	m_primitives["Sphere"] = std::make_shared<OpenGLPrimitive>(sphereData.first, sphereData.second);
	
    std::vector<OpenGLVertex> cubeVertices = PrimitiveGenerator::GenerateCube();
	m_primitives["Cube"] = std::make_shared<OpenGLPrimitive>(cubeVertices);
}

std::string OpenGLPrimitiveManager::LoadPrimitive(std::string filePath)
{
	if (m_primitives.contains(filePath))
	{
		return filePath;
	}
	
	tinyobj::ObjReaderConfig objReaderConfig;
	objReaderConfig.mtl_search_path = "./";
	tinyobj::ObjReader objReader;
	if (!objReader.ParseFromFile(filePath, objReaderConfig))
	{
		if (!objReader.Error().empty())
		{
			std::cerr<<"OpenGLPrimitiveManager|LoadPrimitive|TinyObjReader: "<<objReader.Error()<<"\n";
		}
		return "";
	}

	if (!objReader.Warning().empty())
	{
		std::cout<<"OpenGLPrimitiveManager|LoadPrimitive|TinyObjReader: "<<objReader.Warning()<<"\n";
	}

	auto& attrib = objReader.GetAttrib();
	auto& shapes = objReader.GetShapes();
	auto& materials = objReader.GetMaterials();

	if (materials.size() > 1)
	{
		std::cout<<"OpenGLPrimitiveManager|LoadPrimitive: More than 2 materials per .obj specified. Currently no support for per-face materials\n";
		return "";
	}

	std::vector<OpenGLVertex> vertices;

	// Loop over shapes
	for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
	{
		// Loop over faces
		size_t index_offset = 0;
		for (size_t faceIndex = 0; faceIndex < shapes[shapeIndex].mesh.num_face_vertices.size(); faceIndex++)
		{
			size_t faceVertexCount = size_t(shapes[shapeIndex].mesh.num_face_vertices[faceIndex]);

			// Loop over vertices in the face.
			for (size_t v = 0; v < faceVertexCount; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[shapeIndex].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
				tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
				tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				if (idx.normal_index < 0) {
					std::cout<<"OpenGLPrimitiveManager|LoadPrimitive: No normals present, currently require normals\n";
					return "";
				}
				
				tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
				tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
				tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
					tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

					vertices.emplace_back(glm::vec3(vx, vy, vz), glm::vec3(nx, ny, nz), glm::vec2(tx, ty), true);
				}
				else
				{
					vertices.emplace_back(glm::vec3(vx, vy, vz), glm::vec3(nx, ny, nz), glm::vec2(0, 0), false);
				}
			}
			index_offset += faceVertexCount;

			// per-face material
			shapes[shapeIndex].mesh.material_ids[faceIndex];
		}
	}

	m_primitives[filePath] = std::make_shared<OpenGLPrimitive>(vertices);
	return filePath;
}

void OpenGLPrimitiveManager::DrawPrimitive(std::string fileName)
{
	if (!m_primitives.contains(fileName))
	{
		std::cout<<"OpenGLPrimitiveManager|DrawPrimitive| Primitive for "<<fileName<<" not found\n";
		return;
	}
	m_primitives[fileName]->Draw();
}

std::vector<std::string> OpenGLPrimitiveManager::GetPrimitiveNames() const
{
	std::vector<std::string> names;
	for (const auto& name : m_primitives | std::views::keys)
	{
		names.push_back(name);
	}
	return names;
}
