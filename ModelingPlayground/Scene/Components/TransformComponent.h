#pragma once

#include "glm/glm.hpp"

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../Utils/DataBinding.h"

class TransformComponent : public Component
{
public:
    TransformComponent();

    void RenderInspector() override;

    glm::mat4 GetCumulativeModelMatrix() const;
    glm::mat4 GetInverseCumulativeModelMatrix() const;

    void SetParentCumulativeModelMatrix(glm::mat4 parentCumulativeModelMatrix);
    glm::mat4 GetParentCumulativeModelMatrix() const;
    DataBinding<glm::mat4>& GetParentCumulativeModelMatrixDataBinding();

    const glm::mat4& GetLocalModelMatrix() const;
    DataBinding<glm::mat4>& GetLocalModelMatrixDataBinding();

    const glm::vec3& GetPosition() const;
    DataBinding<glm::vec3>& GetPositionDataBinding();

    const glm::vec3& GetRotation() const;
    DataBinding<glm::vec3>& GetRotationDataBinding();

    const glm::vec3& GetScale() const;
    DataBinding<glm::vec3>& GetScaleDataBinding();

    const glm::vec3& GetLocalXUnitVector() const;
    DataBinding<glm::vec3>& GetLocalXUnitVectorDataBinding();

    glm::vec3 GetWorldSpacePosition() const;
    glm::vec3 GetWorldSpaceXUnitVector() const;

    friend void to_json(nlohmann::json& json, const TransformComponent& transformComponent);
    friend void from_json(const nlohmann::json& json, TransformComponent& transformComponent);

private:
    void UpdateLocalModelMatrix();
    void UpdateLocalXUnitVector();

    DataBinding<glm::mat4> m_parentCumulativeModelMatrix;
    DataBinding<glm::mat4> m_localModelMatrix;

    DataBinding<glm::vec3> m_position;
    DataBinding<glm::vec3> m_rotation;
    DataBinding<glm::vec3> m_scale;
    DataBinding<glm::vec3> m_localXUnitVector;
};
