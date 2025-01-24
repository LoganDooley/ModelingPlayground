#include "SceneViewWindow.h"

#include <iostream>
#include <ostream>

#include "imgui.h"
#include "../ShaderLoader.h"

SceneViewWindow::SceneViewWindow()
{
	InitializeOpenGLObjects();
}

SceneViewWindow::~SceneViewWindow()
{
}

void SceneViewWindow::Render()
{
	// Draw triangle to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glViewport(0, 0, 600, 600);
	glBindVertexArray(m_triangleVAO);
	glUseProgram(m_defaultShader);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the scene view!");
	{
		ImGui::BeginChild("GameRender");
		ImGui::Image((ImTextureID)m_framebufferTexture, ImVec2(600, 600), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}

void SceneViewWindow::InitializeOpenGLObjects()
{
	// Create triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	glGenVertexArrays(1, &m_triangleVAO);
	glBindVertexArray(m_triangleVAO);

	glGenBuffers(1, &m_triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create shader
	m_defaultShader = ShaderLoader::createShaderProgram("Shaders/default.vert", "Shaders/default.frag");

	// Create framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// Create framebuffer texture for color
	glGenTextures(1, &m_framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Bind color texture to framebuffer for the color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

	// Create framebuffer renderbuffer for depth and stencil
	glGenRenderbuffers(1, &m_framebufferRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_framebufferRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 600, 600);

	// Bind renderbuffer for the depth and stencil attachments
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_framebufferRenderbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << '\n';
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
