#include "OpenGLPrimitive.h"

#include <iostream>

OpenGLPrimitive::OpenGLPrimitive():
	m_vbo(-1),
	m_vao(-1),
	m_ebo(-1)
{
}

OpenGLPrimitive::OpenGLPrimitive(const std::vector<float>& vertices, const std::vector<int>& indices,
                                 const std::vector<VertexAttribute>& vertexAttributeLayout):
	m_vbo(0),
	m_vao(0),
	m_ebo(0),
	m_vertexAttributeLayout(vertexAttributeLayout),
	m_vertices(vertices),
	m_indices(indices)
{
	if (vertices.empty())
	{
		std::cout << "OpenGLPrimitive: Created with no vertices!\n";
		return;
	}

	if (indices.empty())
	{
		std::cout << "OpenGLPrimitive: Created with no indices!\n";
		return;
	}

	if (!IsVertexAttributeLayoutSupported())
	{
		std::cout << "OpenGLPrimitive: Vertex layout is not supported!\n";
		return;
	}

	CreateOpenGLObjects();
}

OpenGLPrimitive::~OpenGLPrimitive()
{
	if (m_vbo > 0)
	{
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_vao > 0)
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_ebo > 0)
	{
		glDeleteBuffers(1, &m_ebo);
	}
}

void OpenGLPrimitive::CreateOpenGLObjects()
{
	int vertexLength = 0;
	for (auto vertexAttribute : m_vertexAttributeLayout)
	{
		switch (vertexAttribute)
		{
		case VertexAttribute::Position:
		case VertexAttribute::Normal:
			vertexLength += 3;
			break;
		case VertexAttribute::UV:
			vertexLength += 2;
			break;
		}
	}

	GLsizei vertexStride = vertexLength * sizeof(float);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

	size_t attributeOffset = 0;

	for (unsigned int i = 0; i < m_vertexAttributeLayout.size(); i++)
	{
		glVertexAttribPointer(i, GetSize(m_vertexAttributeLayout[i]), GL_FLOAT, GL_FALSE, vertexStride,
		                      (void*)attributeOffset);
		glEnableVertexAttribArray(i);
		attributeOffset += GetSize(m_vertexAttributeLayout[i]) * sizeof(float);
	}

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), m_indices.data(), GL_STATIC_DRAW);
}

void OpenGLPrimitive::Draw() const
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

bool OpenGLPrimitive::IsVertexAttributeLayoutSupported() const
{
	if (m_vertexAttributeLayout.size() >= 2)
	{
		if (m_vertexAttributeLayout[0] != VertexAttribute::Position)
		{
			return false;
		}
		if (m_vertexAttributeLayout[1] != VertexAttribute::Normal)
		{
			return false;
		}
		if (m_vertexAttributeLayout.size() >= 3 && m_vertexAttributeLayout[2] != VertexAttribute::UV)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}
