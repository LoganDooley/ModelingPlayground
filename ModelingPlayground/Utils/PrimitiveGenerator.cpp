#include "PrimitiveGenerator.h"

#include <iostream>
#include <glm/geometric.hpp>
#include <glm/ext/scalar_constants.hpp>

std::pair<std::vector<float>, std::vector<int>> PrimitiveGenerator::GenerateSphere(int latitudinalResolution,
	int longitudinalResolution)
{
	if (latitudinalResolution < 3)
	{
		std::cout <<
			"PrimitiveGenerator|GenerateSphere: Warning! latitudinalResolution must be at least 3, generating a "
			"sphere with latitudinalResolution = 3\n";
		latitudinalResolution = 3;
	}

	if (longitudinalResolution < 2)
	{
		std::cout <<
			"PrimitiveGenerator|GenerateSphere: Warning! longitudinalResolution must be at least 2, generating a"
			"sphere with longitudinalResolution = 2\n";
		longitudinalResolution = 2;
	}

	std::vector<float> vertices;
	std::vector<int> indices;

	// First generate vertices
	for (int latitudeIndex = 0; latitudeIndex <= latitudinalResolution; latitudeIndex++)
	{
		// Generate a ring of points at this latitude
		float phi = glm::pi<float>() * (1 - static_cast<float>(latitudeIndex) / static_cast<float>(
			latitudinalResolution));

		for (int longitudeIndex = 0; longitudeIndex <= longitudinalResolution; longitudeIndex++)
		{
			float theta = 2 * glm::pi<float>() * (static_cast<float>(longitudeIndex) / static_cast<float>(
				longitudinalResolution));
			glm::vec3 position = SphericalToCartesian(phi, theta, 0.5f);
			glm::vec3 normal = normalize(position);
			float u = static_cast<float>(latitudeIndex) / static_cast<float>(latitudinalResolution);
			float v = static_cast<float>(longitudeIndex) / static_cast<float>(longitudinalResolution);
			EmplaceVertex(vertices, position, normal, glm::vec2(u, v));
		}
	}

	for (int latitudeIndex = 0; latitudeIndex < latitudinalResolution; latitudeIndex++)
	{
		for (int longitudeIndex = 0; longitudeIndex < longitudinalResolution; longitudeIndex++)
		{
			if (latitudeIndex == 0)
			{
				// Generate one triangle
				indices.emplace_back(GetSphereVertexIndex(0, longitudeIndex,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(1, longitudeIndex + 1,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(1, longitudeIndex,
				                                          latitudinalResolution));
			}
			else if (latitudeIndex == latitudinalResolution - 1)
			{
				// Generate one triangle
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex + 1,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex + 1, longitudeIndex,
				                                          latitudinalResolution));
			}
			else
			{
				// Generate two triangles
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex + 1,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex + 1, longitudeIndex,
				                                          latitudinalResolution));

				indices.emplace_back(GetSphereVertexIndex(latitudeIndex + 1, longitudeIndex,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex + 1,
				                                          latitudinalResolution));
				indices.emplace_back(GetSphereVertexIndex(latitudeIndex + 1, longitudeIndex + 1,
				                                          latitudinalResolution));
			}
		}
	}

	return {vertices, indices};
}

std::pair<std::vector<float>, std::vector<int>> PrimitiveGenerator::GenerateCube()
{
	std::vector<float> vertices;
	std::vector<int> indices;

	GenerateCubeFace(vertices, indices, glm::vec3(1, 0, 0));
	GenerateCubeFace(vertices, indices, glm::vec3(-1, 0, 0));
	GenerateCubeFace(vertices, indices, glm::vec3(0, 1, 0));
	GenerateCubeFace(vertices, indices, glm::vec3(0, -1, 0));
	GenerateCubeFace(vertices, indices, glm::vec3(0, 0, 1));
	GenerateCubeFace(vertices, indices, glm::vec3(0, 0, -1));

	return {vertices, indices};
}

glm::vec3 PrimitiveGenerator::SphericalToCartesian(float phi, float theta, float r)
{
	// Using y up coordinates
	float z = r * std::sin(phi) * std::cos(theta);
	float x = r * std::sin(phi) * std::sin(theta);
	float y = r * std::cos(phi);

	return glm::vec3(x, y, z);
}

int PrimitiveGenerator::GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution)
{
	return longitudeIndex + latitudeIndex * (latitudinalResolution + 1);
}

void PrimitiveGenerator::GenerateCubeFace(std::vector<float>& vertices, std::vector<int>& indices, glm::vec3 faceNormal)
{
	int indexA;
	int indexB;

	if (faceNormal.x < 0)
	{
		indexA = 2;
		indexB = 1;
	}
	else if (faceNormal.x > 0)
	{
		indexA = 1;
		indexB = 2;
	}
	else if (faceNormal.y < 0)
	{
		indexA = 0;
		indexB = 2;
	}
	else if (faceNormal.y > 0)
	{
		indexA = 2;
		indexB = 0;
	}
	else if (faceNormal.z < 0)
	{
		indexA = 1;
		indexB = 0;
	}
	else
	{
		indexA = 0;
		indexB = 1;
	}

	glm::vec3 v0 = faceNormal / 2.f;
	v0[indexA] = 0.5;
	v0[indexB] = 0.5;

	glm::vec3 v1 = faceNormal / 2.f;
	v1[indexA] = 0.5;
	v1[indexB] = -0.5;

	glm::vec3 v2 = faceNormal / 2.f;
	v2[indexA] = -0.5;
	v2[indexB] = -0.5;

	glm::vec3 v3 = faceNormal / 2.f;
	v3[indexA] = -0.5;
	v3[indexB] = 0.5;

	int vertexCount = vertices.size() / 8;

	EmplaceVertex(vertices, v0, faceNormal, glm::vec2(1, 0));
	EmplaceVertex(vertices, v1, faceNormal, glm::vec2(0, 0));
	EmplaceVertex(vertices, v2, faceNormal, glm::vec2(0, 1));
	EmplaceVertex(vertices, v3, faceNormal, glm::vec2(1, 1));

	indices.emplace_back(vertexCount);
	indices.emplace_back(vertexCount + 3);
	indices.emplace_back(vertexCount + 1);

	indices.emplace_back(vertexCount + 1);
	indices.emplace_back(vertexCount + 3);
	indices.emplace_back(vertexCount + 2);
}

void PrimitiveGenerator::EmplaceVertex(std::vector<float>& vertices, glm::vec3 position, glm::vec3 normal,
                                       glm::vec2 uv)
{
	vertices.emplace_back(position.x);
	vertices.emplace_back(position.y);
	vertices.emplace_back(position.z);
	vertices.emplace_back(normal.x);
	vertices.emplace_back(normal.y);
	vertices.emplace_back(normal.z);
	vertices.emplace_back(uv.x);
	vertices.emplace_back(uv.y);
}
