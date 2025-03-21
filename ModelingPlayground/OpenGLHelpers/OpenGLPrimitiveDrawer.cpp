#include "OpenGLPrimitiveDrawer.h"

#include <utility>
#include <vector>
#include <glm/vec3.hpp>

#include "../../Utils/PrimitiveGenerator.h"

OpenGLPrimitiveDrawer::OpenGLPrimitiveDrawer():
	m_triangleVBO(-1),
	m_triangleVAO(-1),
	m_sphereVBO(-1),
	m_sphereVAO(-1),
	m_sphereEBO(-1),
	m_sphereIndexCount(-1),
	m_cubeVBO(-1),
	m_cubeVAO(-1),
	m_cubeEBO(-1),
	m_cubeVertexCount(-1)
{
    
}

void OpenGLPrimitiveDrawer::GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution)
{
	// Initialize Triangle
    float triangleData[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_triangleVAO);
    glBindVertexArray(m_triangleVAO);

    GLuint triangleVBO;
    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    // Initialize Sphere
    std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<int>> sphereData = PrimitiveGenerator::GenerateSphere(sphereLatitudinalResolution, sphereLongitudinalResolution);

	std::vector<glm::vec3> sphereVertices = std::get<0>(sphereData);
	std::vector<glm::vec3> sphereNormals = std::get<1>(sphereData);
	std::vector<glm::vec2> sphereTexCoords = std::get<2>(sphereData);
	std::vector<int> sphereIndices = std::get<3>(sphereData);
	
	std::vector<float> sphereVBOData;
	for (int i = 0; i<sphereVertices.size(); i++)
	{
		sphereVBOData.push_back(sphereVertices[i].x);
		sphereVBOData.push_back(sphereVertices[i].y);
		sphereVBOData.push_back(sphereVertices[i].z);
		
		sphereVBOData.push_back(sphereNormals[i].x);
		sphereVBOData.push_back(sphereNormals[i].y);
		sphereVBOData.push_back(sphereNormals[i].z);
		
		sphereVBOData.push_back(sphereTexCoords[i].x);
		sphereVBOData.push_back(sphereTexCoords[i].y);
	}
	
	m_sphereIndexCount = static_cast<GLuint>(sphereIndices.size());
	
    glGenVertexArrays(1, &m_sphereVAO);
    glBindVertexArray(m_sphereVAO);
	
    glGenBuffers(1, &m_sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVBOData.size() * sizeof(float), sphereVBOData.data(), GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphereIndexCount * sizeof(int), sphereIndices.data(), GL_STATIC_DRAW);

    // Initialize Cube
    std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::vec2>> cubeData = PrimitiveGenerator::GenerateCube();

	std::vector<glm::vec3> cubeVertices = std::get<0>(cubeData);
	std::vector<glm::vec3> cubeNormals = std::get<1>(cubeData);
	std::vector<glm::vec2> cubeTexCoords = std::get<2>(cubeData);

	std::vector<float> cubeVBOData;

	for (int i = 0; i<cubeVertices.size(); i++)
	{
		cubeVBOData.push_back(cubeVertices[i].x);
		cubeVBOData.push_back(cubeVertices[i].y);
		cubeVBOData.push_back(cubeVertices[i].z);

		cubeVBOData.push_back(cubeNormals[i].x);
		cubeVBOData.push_back(cubeNormals[i].y);
		cubeVBOData.push_back(cubeNormals[i].z);
		
		cubeVBOData.push_back(cubeTexCoords[i].x);
		cubeVBOData.push_back(cubeTexCoords[i].y);
	}
	
	m_cubeVertexCount = static_cast<GLuint>(cubeVertices.size());
	
    glGenVertexArrays(1, &m_cubeVAO);
    glBindVertexArray(m_cubeVAO);
	
    glGenBuffers(1, &m_cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVBOData.size() * sizeof(float), cubeVBOData.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void OpenGLPrimitiveDrawer::DrawPrimitive(PrimitiveType primitiveType)
{
	switch (primitiveType)
	{
	case PrimitiveType::Triangle:
		glBindVertexArray(m_triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case PrimitiveType::Sphere:
		glBindVertexArray(m_sphereVAO);
		glDrawElements(GL_TRIANGLES, m_sphereIndexCount, GL_UNSIGNED_INT, 0);
		break;
	case PrimitiveType::Cube:
		glBindVertexArray(m_cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, m_cubeVertexCount);
		break;
	}
	return;
}
