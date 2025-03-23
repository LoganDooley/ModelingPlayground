#include "TransformComponent.h"

#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "imgui.h"
#include "../../OpenGLHelpers/OpenGLRenderer.h"
#include "../../Utils/PropertyDrawer.h"

TransformComponent::TransformComponent():
	m_parentCumulativeModelMatrix(glm::mat4(1)),
	m_localModelMatrix(glm::mat4(1)),
	m_position(glm::vec3(0)),
	m_rotation(glm::vec3(0)),
	m_scale(glm::vec3(1)),
	m_localXUnitVector(glm::vec3(1, 0, 0))

{
}

void TransformComponent::RenderInspector()
{
	ImGuiTreeNodeFlags transformHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", transformHeaderFlags))
	{
		// Position
		if (PropertyDrawer::DrawVec3fDrag("Position", m_position, 0.01f))
		{
			UpdateLocalModelMatrix();
		}

		// Rotation
		if (PropertyDrawer::DrawVec3fDrag("Rotation", m_rotation, 1.0f))
		{
			glm::vec3 rotationModulus = m_rotation.GetData();
			rotationModulus.x = std::fmodf(rotationModulus.x, 360.0f);
			rotationModulus.y = std::fmodf(rotationModulus.y, 360.0f);
			rotationModulus.z = std::fmodf(rotationModulus.z, 360.0f);
			m_rotation.SetAndNotify(rotationModulus);

			UpdateLocalModelMatrix();
			UpdateLocalXUnitVector();
		}

		// Scale
		if (PropertyDrawer::DrawVec3fDrag("Scale", m_scale, 0.01f))
		{
			UpdateLocalModelMatrix();
		}
	}
}

glm::mat4 TransformComponent::GetCumulativeModelMatrix() const
{
	return m_parentCumulativeModelMatrix.GetData() * m_localModelMatrix.GetData();
}

void TransformComponent::SetParentCumulativeModelMatrix(glm::mat4 parentCumulativeModelMatrix)
{
	m_parentCumulativeModelMatrix.SetAndNotify(parentCumulativeModelMatrix, true);
}

glm::mat4 TransformComponent::GetParentCumulativeModelMatrix() const
{
	return m_parentCumulativeModelMatrix.GetData();
}

DataBinding<glm::mat4>& TransformComponent::GetParentCumulativeModelMatrixDataBinding()
{
	return m_parentCumulativeModelMatrix;
}

const glm::mat4& TransformComponent::GetLocalModelMatrix() const
{
	return m_localModelMatrix.GetData();
}

DataBinding<glm::mat4>& TransformComponent::GetLocalModelMatrixDataBinding()
{
	return m_localModelMatrix;
}

const glm::vec3& TransformComponent::GetPosition() const
{
	return m_position.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetPositionDataBinding()
{
	return m_position;
}

const glm::vec3& TransformComponent::GetRotation() const
{
	return m_rotation.GetData();
}

const glm::vec3& TransformComponent::GetScale() const
{
	return m_scale.GetData();
}

const glm::vec3& TransformComponent::GetLocalXUnitVector() const
{
	return m_localXUnitVector.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetLocalXUnitVectorDataBinding()
{
	return m_localXUnitVector;
}

void TransformComponent::UpdateLocalModelMatrix()
{
	// scale
	glm::mat4 scaleMatrix = scale(glm::mat4(1), m_scale.GetData());

	// rotation
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.GetData().x),
	                                              glm::radians(m_rotation.GetData().y),
	                                              glm::radians(m_rotation.GetData().z));

	// translation
	glm::mat4 translationMatrix = translate(glm::mat4(1), m_position.GetData());

	// model matrix
	m_localModelMatrix.SetAndNotify(translationMatrix * rotationMatrix * scaleMatrix);
}

void TransformComponent::UpdateLocalXUnitVector()
{
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.GetData().x),
	                                              glm::radians(m_rotation.GetData().y),
	                                              glm::radians(m_rotation.GetData().z));
	m_localXUnitVector.SetAndNotify(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}
