#include "SceneViewWindow.h"

#include <iostream>
#include <ostream>
#include <stack>

#include "imgui.h"
#include "../ShaderLoader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Scene.h"
#include "../../Scene/Components/ClearColorComponent.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "glm/glm.hpp"

SceneViewWindow::SceneViewWindow(const std::shared_ptr<Scene>& scene, const std::shared_ptr<InputManager>& inputManager):
	m_scene(scene),
	m_camera(std::make_unique<SceneViewCamera>(inputManager, glm::uvec2(1, 1)))
{
	InitializeOpenGLObjects();
}

void SceneViewWindow::Render()
{
	DrawScene();
	
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	m_camera->SetScreenSize(glm::uvec2(windowSize.x, windowSize.x / m_camera->GetAspectRatio()));
	ImGui::Text("I'm the scene view!");
	{
		ImGui::BeginChild("GameRender");
		ImGui::Image(m_camera->GetFramebuffer(), ImVec2(static_cast<float>(m_camera->GetScreenSize().x), static_cast<float>(m_camera->GetScreenSize().y)),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}

void SceneViewWindow::Update(double seconds)
{
	m_camera->Update(seconds);
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
	m_cameraMatrixLocation = glGetUniformLocation(m_defaultShader, "cameraMatrix");
}

void SceneViewWindow::DrawScene() const
{
	// Switch to offscreen framebuffer
	m_camera->BindFramebuffer();
	// Adjust the viewport size
	m_camera->SetViewport();

	glUniformMatrix4fv(m_cameraMatrixLocation, 1, false, &m_camera->GetCameraMatrix()[0][0]);

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

void SceneViewWindow::ProcessObject(const Object& object, glm::mat4& cumulativeModelMatrix) const
{
	std::vector<std::shared_ptr<PrimitiveComponent>> primitiveComponents = object.GetComponents<PrimitiveComponent>();
	if (primitiveComponents.size() == 1)
	{
		std::vector<std::shared_ptr<TransformComponent>> transformComponents = object.GetComponents<TransformComponent>();
		if (transformComponents.size() == 1)
		{
			DrawMesh(*primitiveComponents[0], *transformComponents[0], cumulativeModelMatrix);
		}
	}

	std::vector<std::shared_ptr<ClearColorComponent>> clearColorComponents = object.GetComponents<ClearColorComponent>();
	if (clearColorComponents.size() == 1)
	{
		glm::vec4 clearColor = clearColorComponents[0]->GetClearColor();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void SceneViewWindow::DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
	glm::mat4& cumulativeModelMatrix) const
{
	cumulativeModelMatrix = cumulativeModelMatrix * transformComponent.GetModelMatrix();
	glBindVertexArray(m_triangleVAO);
	glUseProgram(m_defaultShader);
	glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &cumulativeModelMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}