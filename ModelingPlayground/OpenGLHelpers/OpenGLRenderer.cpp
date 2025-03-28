#include "OpenGLRenderer.h"

#include <queue>
#include <stack>

#include "../../Scene/Object.h"
#include "../../Scene/Components/MaterialComponent.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../Application/Window/SceneViewCamera.h"

OpenGLRenderer::OpenGLRenderer():
	m_defaultShader(std::make_shared<OpenGLShader>()),
	m_sceneHierarchy(std::make_shared<SceneHierarchy>()),
	m_openGLPrimitiveManager(std::make_shared<OpenGLPrimitiveManager>()),
	m_openGLLightContainer(std::make_unique<OpenGLLightContainer>())
{
}

void OpenGLRenderer::Initialize()
{
	m_defaultShader->LoadShader("Shaders/default.vert", "Shaders/default.frag");

	// Initialize shader
	m_defaultShader->RegisterUniformVariable("modelMatrix");
	m_defaultShader->RegisterUniformVariable("inverseTransposeModelMatrix");
	m_defaultShader->RegisterUniformVariable("cameraMatrix");
	m_defaultShader->RegisterUniformVariable("cameraPosition");
	m_defaultShader->RegisterUniformVariable("ambientColor");
	m_defaultShader->RegisterUniformVariable("materialColor");
	m_defaultShader->RegisterUniformVariable("roughness");
	m_defaultShader->RegisterUniformVariable("metallic");

	m_defaultShader->RegisterUniformBufferObject("LightsBlock", 64 * 250 + 4, 0);

	m_openGLPrimitiveManager->GeneratePrimitives(10, 10);
	m_openGLLightContainer->Initialize(m_defaultShader);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::SetCamera(std::shared_ptr<SceneViewCamera> camera)
{
	m_camera = camera;
}

void OpenGLRenderer::SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy)
{
	m_sceneHierarchy = sceneHierarchy;

	m_openGLLightContainer->SetSceneHierarchy(m_sceneHierarchy);

	m_sceneHierarchy->SubscribeToSceneNodeAdded([this](const std::shared_ptr<SceneNode>& newSceneNode)
	{
		OnSceneNodeAdded(newSceneNode);
	});
}

void OpenGLRenderer::RenderSceneHierarchy() const
{
	// Switch to offscreen framebuffer
	m_camera->BindFramebuffer();
	// Adjust the viewport size
	m_camera->SetViewport();

	// Bind shader
	m_defaultShader->BindShader();

	// Set camera uniforms
	// TODO: Only set these if dirty
	m_defaultShader->SetUniform<glm::mat4>("cameraMatrix", false, m_camera->GetCameraMatrix());
	m_defaultShader->SetUniform<glm::vec3>("cameraPosition", m_camera->GetCameraPosition());

	// DFS draw objects
	std::stack<std::shared_ptr<SceneNode>> traversal;
	traversal.push(m_sceneHierarchy->GetRootSceneNode());
	while (!traversal.empty())
	{
		std::shared_ptr<SceneNode> sceneNodeToProcess = traversal.top();
		traversal.pop();
		ProcessObject(sceneNodeToProcess->GetObject());

		// Add children to stack in reverse order
		const std::vector<std::shared_ptr<SceneNode>>& children = sceneNodeToProcess->GetChildren();
		for (int i = static_cast<int>(children.size()) - 1; i >= 0; i--)
		{
			traversal.push(children[i]);
		}
	}

	// Restore framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<OpenGLPrimitiveManager> OpenGLRenderer::GetOpenGLPrimitiveManager() const
{
	return m_openGLPrimitiveManager;
}

void OpenGLRenderer::ProcessObject(const Object& object) const
{
	if (std::shared_ptr<PrimitiveComponent> primitiveComponent = object.GetFirstComponentOfType<PrimitiveComponent>())
	{
		if (std::shared_ptr<TransformComponent> transformComponent = object.GetFirstComponentOfType<
			TransformComponent>())
		{
			if (std::shared_ptr<MaterialComponent> materialComponent = object.GetFirstComponentOfType<
				MaterialComponent>())
			{
				DrawMesh(*primitiveComponent, *transformComponent, *materialComponent);
			}
		}
	}

	if (std::shared_ptr<OpenGLSettingsComponent> openGLSettingsComponent = object.GetFirstComponentOfType<
		OpenGLSettingsComponent>())
	{
		glm::vec4 clearColor = openGLSettingsComponent->GetClearColor();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_defaultShader->SetUniform<glm::vec3>("ambientColor", openGLSettingsComponent->GetAmbientLight());
	}
}

void OpenGLRenderer::DrawMesh(const PrimitiveComponent& primitiveComponent,
                              const TransformComponent& transformComponent,
                              const MaterialComponent& materialComponent) const
{
	glm::mat4 cumulativeModelMatrix = transformComponent.GetCumulativeModelMatrix();
	m_defaultShader->BindShader();
	m_defaultShader->SetUniform<glm::mat4>("modelMatrix", false, cumulativeModelMatrix);
	glm::mat3 inverseTransposeModelMatrix = transpose(inverse(glm::mat3(cumulativeModelMatrix)));
	m_defaultShader->SetUniform<glm::mat3>("inverseTransposeModelMatrix", false, inverseTransposeModelMatrix);
	m_defaultShader->SetUniform<glm::vec3>("materialColor", materialComponent.GetMaterialColor());
	m_defaultShader->SetUniform<float>("roughness", materialComponent.GetRoughness());
	m_defaultShader->SetUniform<float>("metallic", materialComponent.GetRoughness());

	m_openGLPrimitiveManager->DrawPrimitive(primitiveComponent.GetPrimitiveName());
}

void OpenGLRenderer::OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const
{
	m_openGLLightContainer->TryAddLight(newSceneNode);
}
