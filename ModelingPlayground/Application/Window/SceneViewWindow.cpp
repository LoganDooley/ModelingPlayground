#include "SceneViewWindow.h"

#include <ostream>
#include <stack>

#include "imgui.h"
#include "../../Scene/Object.h"
#include "../../Scene/SceneHierarchy.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "../../Scene/Components/MaterialComponent.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "glm/glm.hpp"

SceneViewWindow::SceneViewWindow(const std::shared_ptr<SceneHierarchy>& scene, const std::shared_ptr<InputManager>& inputManager):
	m_openGLPrimitiveDrawer(std::make_unique<OpenGLPrimitiveDrawer>(10, 10)),
	m_camera(std::make_unique<SceneViewCamera>(inputManager, glm::uvec2(1, 1))),
	m_scene(scene)
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
	// Create shader
	m_defaultShader = std::make_shared<OpenGLShader>("Shaders/default.vert", "Shaders/default.frag");

	m_defaultShader->RegisterUniformVariable("modelMatrix");
	m_defaultShader->RegisterUniformVariable("inverseTransposeModelMatrix");
	m_defaultShader->RegisterUniformVariable("cameraMatrix");
	m_defaultShader->RegisterUniformVariable("ambientColor");
	m_defaultShader->RegisterUniformVariable("materialColor");

	m_scene->CreateLightsContainer(m_defaultShader);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void SceneViewWindow::DrawScene() const
{
	// Switch to offscreen framebuffer
	m_camera->BindFramebuffer();
	// Adjust the viewport size
	m_camera->SetViewport();

	m_defaultShader->SetUniformMatrix4f("cameraMatrix", false, m_camera->GetCameraMatrix());

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
			std::vector<std::shared_ptr<MaterialComponent>> materialComponents = object.GetComponents<MaterialComponent>();
			if (materialComponents.size() == 1)
			{
				DrawMesh(*primitiveComponents[0], *transformComponents[0], *materialComponents[0], cumulativeModelMatrix);
			}
		}
	}

	std::vector<std::shared_ptr<OpenGLSettingsComponent>> openGLSettingsComponents = object.GetComponents<OpenGLSettingsComponent>();
	if (openGLSettingsComponents.size() == 1)
	{
		glm::vec4 clearColor = openGLSettingsComponents[0]->GetClearColor();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void SceneViewWindow::DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
	const MaterialComponent& materialComponent, glm::mat4& cumulativeModelMatrix) const
{
	cumulativeModelMatrix = cumulativeModelMatrix * transformComponent.GetModelMatrix();
	m_defaultShader->BindShader();
	m_defaultShader->SetUniformMatrix4f("modelMatrix", false, cumulativeModelMatrix);
	glm::mat3 inverseTransposeModelMatrix = glm::transpose(glm::inverse(glm::mat3(cumulativeModelMatrix)));
	m_defaultShader->SetUniformMatrix3f("inverseTransposeModelMatrix", false, inverseTransposeModelMatrix);
	glm::vec3 materialColor = materialComponent.GetMaterialColor();
	m_defaultShader->SetUniform3f("materialColor", materialColor);
	m_openGLPrimitiveDrawer->DrawPrimitive(primitiveComponent.GetPrimitiveType());
}