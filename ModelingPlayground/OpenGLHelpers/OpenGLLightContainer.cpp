#include "OpenGLLightContainer.h"

#include <iostream>
#include <ostream>
#include <queue>

#include "../Scene/Object.h"
#include "Lights/OpenGLDirectionalLight.h"
#include "Lights/OpenGLPointLight.h"
#include "Lights/OpenGLSpotLight.h"

OpenGLLightContainer::OpenGLLightContainer():
	m_defaultShader(std::make_shared<OpenGLShader>()),
	m_lights({})
{
}

void OpenGLLightContainer::Initialize(std::shared_ptr<OpenGLShader> defaultShader)
{
	m_defaultShader = defaultShader;

	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightCountOffset, 0);
}

void OpenGLLightContainer::SetSceneHierarchy(const std::shared_ptr<SceneHierarchy>& sceneHierarchy)
{
	m_lights.clear();
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightCountOffset, 0);

	// bfs through hierarchy and add nodes that are lights
	std::queue<std::shared_ptr<SceneNode>> bfs;
	bfs.push(sceneHierarchy->GetRootSceneNode());
	while (!bfs.empty())
	{
		const std::shared_ptr<SceneNode> node = bfs.front();
		bfs.pop();

		TryAddLight(node);

		std::vector<std::shared_ptr<SceneNode>> childNodes = node->GetChildren();
		for (const auto& childNode : childNodes)
		{
			bfs.push(childNode);
		}
	}
}

bool OpenGLLightContainer::TryAddLight(const std::shared_ptr<SceneNode>& lightSceneNode)
{
	LightType lightType = GetLightType(lightSceneNode);
	if (lightType != None)
	{
		return AddLightInternal(lightSceneNode, lightType);
	}
	return false;
}

void OpenGLLightContainer::ClearLights()
{
	m_lights.clear();

	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightCountOffset, 0);
}

bool OpenGLLightContainer::AddLightInternal(const std::shared_ptr<SceneNode>& light, LightType type)
{
	if (m_lights.size() == m_maxLights)
	{
		std::cout << "LightsContainer|AddLight: Could not add light due to exceeding maxLights!\n";
		return false;
	}

	unsigned int newLightIndex = m_lights.size();

	switch (type)
	{
	case None:
		break;
	case Directional:
		m_lights.push_back(std::make_shared<OpenGLDirectionalLight>(m_defaultShader, light, newLightIndex));
		break;
	case Point:
		m_lights.push_back(std::make_shared<OpenGLPointLight>(m_defaultShader, light, newLightIndex));
		break;
	case Spot:
		m_lights.push_back(std::make_shared<OpenGLSpotLight>(m_defaultShader, light, newLightIndex));
		break;
	}

	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightCountOffset, m_lights.size());

	std::shared_ptr<OpenGLLight> newLight = m_lights[newLightIndex];
	light->SubscribeToOnDestroyed([this, newLight]()
	{
		std::erase(m_lights, newLight);
		m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightCountOffset, m_lights.size());
	});

	return true;
}

LightType OpenGLLightContainer::GetLightType(const std::shared_ptr<SceneNode>& lightSceneNode)
{
	if (lightSceneNode->GetObject().GetFirstComponentOfType<DirectionalLightComponent>())
	{
		return Directional;
	}
	if (lightSceneNode->GetObject().GetFirstComponentOfType<PointLightComponent>())
	{
		return Point;
	}
	if (lightSceneNode->GetObject().GetFirstComponentOfType<SpotLightComponent>())
	{
		return Spot;
	}
	return None;
}
