#pragma once

#include "glm/glm.hpp"

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../Utils/DataBinding.h"

class TransformComponent : public Component
{
public:
    TransformComponent();
    TransformComponent(const std::shared_ptr<TransformComponent>& parentTransformComponent);

    void RenderInspector() override;

    DataBinding<glm::mat4>& GetCumulativeModelMatrixDataBinding();
    glm::mat3 GetInverseTransposeCumulativeModelMatrix() const;

    const glm::mat4& GetLocalModelMatrix() const;
    DataBinding<glm::mat4>& GetLocalModelMatrixDataBinding();

    const glm::vec3& GetPosition() const;
    DataBinding<glm::vec3>& GetPositionDataBinding();

    const glm::vec3& GetRotation() const;
    DataBinding<glm::vec3>& GetRotationDataBinding();

    const glm::vec3& GetScale() const;
    DataBinding<glm::vec3>& GetScaleDataBinding();

    glm::vec3 GetWorldSpacePosition() const;
    DataBinding<glm::vec3>& GetWorldSpaceXUnitVectorDataBinding();

    friend void to_json(nlohmann::json& json, const TransformComponent& transformComponent);
    friend void from_json(const nlohmann::json& json, TransformComponent& transformComponent);

private:
    void UpdateLocalModelMatrix();
    void UpdateCumulativeModelMatrix();
    void UpdateWorldXUnitVector();

    std::weak_ptr<TransformComponent> m_parentTransformComponent;
    DataBinding<glm::mat4> m_localModelMatrix;
    DataBinding<glm::mat4> m_cumulativeModelMatrix;

    DataBinding<glm::vec3> m_position;
    DataBinding<glm::vec3> m_rotation;
    DataBinding<glm::vec3> m_scale;
    DataBinding<glm::vec3> m_worldXUnitVector;
};
