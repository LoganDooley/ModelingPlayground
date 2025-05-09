#include "TransformComponent.h"

#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "imgui.h"
#include "../../Application/Rendering/Renderers/OpenGLRenderer.h"
#include "../../Utils/PropertyDrawer.h"

TransformComponent::TransformComponent():
    TransformComponent(nullptr)
{
}

TransformComponent::TransformComponent(const std::shared_ptr<TransformComponent>& parentTransformComponent):
    m_parentTransformComponent(parentTransformComponent),
    m_localModelMatrix(glm::mat4(1)),
    m_cumulativeModelMatrix(glm::mat4(1)),
    m_position(glm::vec3(0)),
    m_rotation(glm::vec3(0)),
    m_scale(glm::vec3(1)),
    m_worldXUnitVector(glm::vec3(1, 0, 0))
{
    m_position.Subscribe(this, [this](const glm::vec3&, glm::vec3)
    {
        UpdateLocalModelMatrix();
    });

    m_rotation.Subscribe(this, [this](const glm::vec3&, glm::vec3)
    {
        UpdateLocalModelMatrix();
    });

    m_scale.Subscribe(this, [this](const glm::vec3&, glm::vec3)
    {
        UpdateLocalModelMatrix();
    });

    m_localModelMatrix.Subscribe(this, [this](const glm::mat4&, glm::mat4)
    {
        UpdateCumulativeModelMatrix();
    });

    if (std::shared_ptr<TransformComponent> parentTransform = m_parentTransformComponent.lock())
    {
        parentTransform->GetCumulativeModelMatrixDataBinding().Subscribe(this, [this](const glm::mat4&, glm::mat4)
        {
            UpdateCumulativeModelMatrix();
        });
    }

    m_cumulativeModelMatrix.Subscribe(this, [this](const glm::mat4&, glm::mat4)
    {
        UpdateWorldXUnitVector();
    });
}

void TransformComponent::RenderInspector()
{
    ImGuiTreeNodeFlags transformHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Transform", transformHeaderFlags))
    {
        // Position
        PropertyDrawer::DrawVec3fDrag("Position", m_position, 0.01f);

        // Rotation
        if (PropertyDrawer::DrawVec3fDrag("Rotation", m_rotation, 1.0f))
        {
            glm::vec3 rotationModulus = m_rotation.GetData();
            rotationModulus.x = std::fmodf(rotationModulus.x, 360.0f);
            rotationModulus.y = std::fmodf(rotationModulus.y, 360.0f);
            rotationModulus.z = std::fmodf(rotationModulus.z, 360.0f);
            m_rotation.SetAndNotify(rotationModulus);
        }

        // Scale
        PropertyDrawer::DrawVec3fDrag("Scale", m_scale, 0.01f);
    }
}

DataBinding<glm::mat4>& TransformComponent::GetCumulativeModelMatrixDataBinding()
{
    return m_cumulativeModelMatrix;
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

DataBinding<glm::vec3>& TransformComponent::GetRotationDataBinding()
{
    return m_rotation;
}

const glm::vec3& TransformComponent::GetScale() const
{
    return m_scale.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetScaleDataBinding()
{
    return m_scale;
}

glm::vec3 TransformComponent::GetWorldSpacePosition() const
{
    if (std::shared_ptr<TransformComponent> parentTransformComponent = m_parentTransformComponent.lock())
    {
        return parentTransformComponent->GetCumulativeModelMatrixDataBinding().GetData() * glm::vec4(
            m_position.GetData(), 1);
    }
    return m_position.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetWorldSpaceXUnitVectorDataBinding()
{
    return m_worldXUnitVector;
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

void TransformComponent::UpdateCumulativeModelMatrix()
{
    if (std::shared_ptr<TransformComponent> parentTransformComponent = m_parentTransformComponent.lock())
    {
        m_cumulativeModelMatrix.SetAndNotify(
            parentTransformComponent->GetCumulativeModelMatrixDataBinding().GetData() * m_localModelMatrix.GetData());
    }
}

void TransformComponent::UpdateWorldXUnitVector()
{
    m_worldXUnitVector.SetAndNotify(
        glm::normalize(glm::vec3(m_cumulativeModelMatrix.GetData() * glm::vec4(1, 0, 0, 0))));
}
