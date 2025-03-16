#pragma once

#include "glm/glm.hpp"

#include "Component.h"
#include "../../Utils/DataBinding.h"
#include "../../nlohmann/json_fwd.hpp"

class TransformComponent : public Component
{
public:
    TransformComponent();
    
    void RenderInspector() override;

    const glm::mat4& GetModelMatrix() const;
    const glm::vec3& GetPosition() const;
    DataBinding<glm::vec3>& GetPositionDataBinding();
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetLocalXUnitVector() const;
    DataBinding<glm::vec3>& GetLocalXUnitVectorDataBinding();
    
    friend void to_json(nlohmann::json& json, const std::shared_ptr<TransformComponent>& transformComponent);
    friend void from_json(const nlohmann::json& json, std::shared_ptr<TransformComponent>& transformComponent);
    
private:
    void UpdateModelMatrix();
    void UpdateLocalXUnitVector();

    DataBinding<glm::vec3> m_position;
    DataBinding<glm::vec3> m_rotation;
    DataBinding<glm::vec3> m_scale;
    DataBinding<glm::vec3> m_localXUnitVector;

    glm::mat4 m_modelMatrix;
};
