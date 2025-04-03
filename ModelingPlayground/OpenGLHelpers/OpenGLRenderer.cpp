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
	m_unidirectionalShadowsShader(std::make_shared<OpenGLShader>()),
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

	m_unidirectionalShadowsShader->LoadShader("Shaders/unidirectionalShadows.vert",
	                                          "Shaders/unidirectionalShadows.frag");

	// Initialize shadow shader
	m_unidirectionalShadowsShader->RegisterUniformVariable("lightMatrix");
	m_unidirectionalShadowsShader->RegisterUniformVariable("modelMatrix");

	m_openGLPrimitiveManager->GeneratePrimitives(10, 10);
	m_openGLLightContainer->Initialize(m_defaultShader);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void OpenGLRenderer::TryUpdateShadowMaps()
{
	m_openGLLightContainer->UpdateDirtyShadowMaps(this);
}

void OpenGLRenderer::RenderScene() const
{
	// Switch to offscreen framebuffer
	m_camera->BindFramebuffer();
	// Adjust the viewport size
	m_camera->SetViewport();

	// Bind shader
	m_defaultShader->BindShader();

	// Set camera uniforms
	m_defaultShader->SetUniform<glm::mat4>("cameraMatrix", false, m_camera->GetCameraMatrix());
	m_defaultShader->SetUniform<glm::vec3>("cameraPosition", m_camera->GetCameraPosition());

	std::shared_ptr<SceneNode> rootSceneNode = m_sceneHierarchy->GetRootSceneNode();
	if (rootSceneNode != nullptr)
	{
		std::shared_ptr<OpenGLSettingsComponent> openGLSettingsComponent = rootSceneNode->GetObject().
			GetFirstComponentOfType<OpenGLSettingsComponent>();
		if (openGLSettingsComponent != nullptr)
		{
			glm::vec4 clearColor = openGLSettingsComponent->GetClearColor();
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_defaultShader->SetUniform<glm::vec3>("ambientColor", openGLSettingsComponent->GetAmbientLight());
		}
	}

	RenderSceneHierarchy(m_defaultShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const
{
	glCullFace(GL_FRONT);

	m_unidirectionalShadowsShader->BindShader();

	m_unidirectionalShadowsShader->SetUniform<glm::mat4>("lightMatrix", false, lightMatrix);

	RenderSceneHierarchy(m_unidirectionalShadowsShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void OpenGLRenderer::RenderSceneHierarchy(std::shared_ptr<OpenGLShader> activeShader) const
{
	// DFS draw objects
	std::stack<std::shared_ptr<SceneNode>> traversal;
	traversal.push(m_sceneHierarchy->GetRootSceneNode());
	while (!traversal.empty())
	{
		std::shared_ptr<SceneNode> sceneNodeToProcess = traversal.top();
		traversal.pop();
		ProcessObject(sceneNodeToProcess->GetObject(), activeShader);

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

void OpenGLRenderer::ProcessObject(const Object& object, std::shared_ptr<OpenGLShader> activeShader) const
{
	if (std::shared_ptr<PrimitiveComponent> primitiveComponent = object.GetFirstComponentOfType<PrimitiveComponent>())
	{
		if (std::shared_ptr<TransformComponent> transformComponent = object.GetFirstComponentOfType<
			TransformComponent>())
		{
			if (std::shared_ptr<MaterialComponent> materialComponent = object.GetFirstComponentOfType<
				MaterialComponent>())
			{
				DrawMesh(*primitiveComponent, *transformComponent, *materialComponent, activeShader);
			}
		}
	}
}

void OpenGLRenderer::DrawMesh(const PrimitiveComponent& primitiveComponent,
                              const TransformComponent& transformComponent,
                              const MaterialComponent& materialComponent,
                              std::shared_ptr<OpenGLShader> activeShader) const
{
	glm::mat4 cumulativeModelMatrix = transformComponent.GetCumulativeModelMatrix();
	activeShader->SetUniform<glm::mat4>("modelMatrix", false, cumulativeModelMatrix);
	if (activeShader == m_defaultShader)
	{
		glm::mat3 inverseTransposeModelMatrix = transpose(inverse(glm::mat3(cumulativeModelMatrix)));
		activeShader->SetUniform<glm::mat3>("inverseTransposeModelMatrix", false, inverseTransposeModelMatrix);
		activeShader->SetUniform<glm::vec4>("materialColor", materialComponent.GetMaterialColor());
		activeShader->SetUniform<float>("roughness", materialComponent.GetRoughness());
		activeShader->SetUniform<float>("metallic", materialComponent.GetRoughness());
	}

	m_openGLPrimitiveManager->DrawPrimitive(primitiveComponent.GetPrimitiveName());
}

void OpenGLRenderer::OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const
{
	m_openGLLightContainer->TryAddLight(newSceneNode);
}
