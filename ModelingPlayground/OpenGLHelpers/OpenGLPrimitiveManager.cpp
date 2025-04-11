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
	std::vector<float> triangleVertices = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5, 1
	};

	std::vector<int> triangleIndices = {
		0, 1, 2
	};

	m_primitives["Triangle"] = std::make_shared<OpenGLPrimitive>(triangleVertices, triangleIndices,
	                                                             std::vector{
		                                                             VertexAttribute::Position, VertexAttribute::Normal,
		                                                             VertexAttribute::UV
	                                                             });


	std::pair<std::vector<float>, std::vector<int>> sphereData = PrimitiveGenerator::GenerateSphere(
		sphereLatitudinalResolution, sphereLongitudinalResolution);
	m_primitives["Sphere"] = std::make_shared<OpenGLPrimitive>(sphereData.first, sphereData.second, std::vector{
		                                                           VertexAttribute::Position, VertexAttribute::Normal,
		                                                           VertexAttribute::UV
	                                                           });

	std::pair<std::vector<float>, std::vector<int>> cubeData = PrimitiveGenerator::GenerateCube();
	m_primitives["Cube"] = std::make_shared<OpenGLPrimitive>(cubeData.first, cubeData.second, std::vector{
		                                                         VertexAttribute::Position, VertexAttribute::Normal,
		                                                         VertexAttribute::UV
	                                                         });
}

void OpenGLPrimitiveManager::AddPrimitive(const std::string& primitiveName, std::shared_ptr<OpenGLPrimitive> primitive)
{
	if (m_primitives.contains(primitiveName))
	{
		std::cout << "OpenGLPrimitiveManager|AddPrimitive: " << primitiveName << " already exists!\n";
		return;
	}
	m_primitives[primitiveName] = primitive;
}

void OpenGLPrimitiveManager::DrawPrimitive(const std::string& primitiveName)
{
	if (!m_primitives.contains(primitiveName))
	{
		std::cout << "OpenGLPrimitiveManager|DrawPrimitive| Primitive for " << primitiveName << " not found\n";
		return;
	}
	m_primitives[primitiveName]->Draw();
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
