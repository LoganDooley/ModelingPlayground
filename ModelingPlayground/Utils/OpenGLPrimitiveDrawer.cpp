#include "OpenGLPrimitiveDrawer.h"

#include <utility>
#include <vector>
#include <glm/vec3.hpp>

#include "PrimitiveGenerator.h"

OpenGLPrimitiveDrawer::OpenGLPrimitiveDrawer(int sphereLatitudinalResolution, int sphereLongitudinalResolution)
{
    // Initialize Triangle
    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &m_triangleVAO);
    glBindVertexArray(m_triangleVAO);

    GLuint triangleVBO;
    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Initialize Sphere
    std::pair<std::vector<glm::vec3>, std::vector<int>> sphereData = PrimitiveGenerator::GenerateSphere(sphereLatitudinalResolution, sphereLongitudinalResolution);

	m_sphereIndexCount = sphereData.second.size();
	
    glGenVertexArrays(1, &m_sphereVAO);
    glBindVertexArray(m_sphereVAO);
	
    glGenBuffers(1, &m_sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereData.first.size() * sizeof(glm::vec3), sphereData.first.data(), GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_sphereEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_sphereIndexCount * sizeof(int), sphereData.second.data(), GL_STATIC_DRAW);

    // Initialize Cube
    std::pair<std::vector<glm::vec3>, std::vector<int>> cubeData = PrimitiveGenerator::GenerateCube();
	
	m_cubeIndexCount = cubeData.second.size();
	
    glGenVertexArrays(1, &m_cubeVAO);
    glBindVertexArray(m_cubeVAO);
	
    glGenBuffers(1, &m_cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeData.first.size() * sizeof(glm::vec3), cubeData.first.data(), GL_STATIC_DRAW);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_cubeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_cubeIndexCount * sizeof(int), cubeData.second.data(), GL_STATIC_DRAW);

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
		glDrawElements(GL_TRIANGLES, m_cubeIndexCount, GL_UNSIGNED_INT, 0);
		break;
	}
	return;
}
