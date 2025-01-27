#include "SceneViewWindow.h"

#include <iostream>
#include <ostream>
#include <stack>

#include "imgui.h"
#include "../ShaderLoader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Components/ClearColorComponent.h"
#include "../../Scene/Components/MeshComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "glm/glm.hpp"

SceneViewWindow::SceneViewWindow(const std::shared_ptr<Scene>& scene):
	m_scene(scene)
{
	InitializeOpenGLObjects();
}

void SceneViewWindow::Render()
{
	DrawScene();
	
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

	m_modelMatrixLocation = glGetUniformLocation(m_defaultShader, "modelMatrix");

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

void SceneViewWindow::DrawScene() const
{
	// Switch to offscreen framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	// Adjust the viewport size
	glViewport(0, 0, 600, 600);

	// DFS draw objects
	std::stack<std::pair<std::shared_ptr<SceneNode>, glm::mat4>> traversal;
	traversal.push({m_scene->GetRootSceneNode(), glm::mat4(1.0f)});
	while (!traversal.empty())
	{
		std::pair<std::shared_ptr<SceneNode>, glm::mat4> sceneNodeToProcess = traversal.top();
		std::shared_ptr<SceneNode> sceneNode = sceneNodeToProcess.first;
		glm::mat4 cumulativeModelMatrix = sceneNodeToProcess.second;
		traversal.pop();
		ProcessObject(sceneNode->GetObject(), cumulativeModelMatrix);

		// Add children to stack in reverse order
		const std::vector<std::shared_ptr<SceneNode>>& children = sceneNode->GetChildren();
		for (int i = children.size() - 1; i >= 0; i--)
		{
			traversal.push({children[i], cumulativeModelMatrix});
		}
	}

	// Restore framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneViewWindow::ProcessObject(const std::shared_ptr<Object>& object, glm::mat4& cumulativeModelMatrix) const
{
	std::vector<std::shared_ptr<MeshComponent>> meshComponents = object->GetComponents<MeshComponent>();
	if (meshComponents.size() == 1)
	{
		std::vector<std::shared_ptr<TransformComponent>> transformComponents = object->GetComponents<TransformComponent>();
		if (transformComponents.size() == 1)
		{
			DrawMesh(meshComponents[0], transformComponents[0], cumulativeModelMatrix);
		}
	}

	std::vector<std::shared_ptr<ClearColorComponent>> clearColorComponents = object->GetComponents<ClearColorComponent>();
	if (clearColorComponents.size() == 1)
	{
		glm::vec4 clearColor = clearColorComponents[0]->GetClearColor();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void SceneViewWindow::DrawMesh(const std::shared_ptr<MeshComponent>& meshComponent,
	const std::shared_ptr<TransformComponent>& transformComponent, glm::mat4& cumulativeModelMatrix) const
{
	cumulativeModelMatrix = cumulativeModelMatrix * transformComponent->GetModelMatrix();
	glBindVertexArray(m_triangleVAO);
	glUseProgram(m_defaultShader);
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &cumulativeModelMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

