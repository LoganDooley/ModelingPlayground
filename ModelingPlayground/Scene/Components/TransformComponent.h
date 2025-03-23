#pragma once

#include "glm/glm.hpp"

#include "Component.h"
#include "../../Utils/DataBinding.h"
#include "../../nlohmann/json_fwd.hpp"

class OpenGLRenderer;

class TransformComponent : public Component
{
public:
    TransformComponent();
    TransformComponent(std::shared_ptr<OpenGLRenderer> openGLRenderer);
    
    void RenderInspector() override;

    void SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer);
    const glm::mat4& GetModelMatrix() const;
    const glm::vec3& GetPosition() const;
    DataBinding<glm::vec3>& GetPositionDataBinding();
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetLocalXUnitVector() const;
    DataBinding<glm::vec3>& GetLocalXUnitVectorDataBinding();
    
    friend void to_json(nlohmann::json& json, const TransformComponent& transformComponent);
    friend void from_json(const nlohmann::json& json, TransformComponent& transformComponent);
    
private:
    void UpdateModelMatrix();
    void UpdateLocalXUnitVector();

    std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
    
    DataBinding<glm::vec3> m_position;
    DataBinding<glm::vec3> m_rotation;
    DataBinding<glm::vec3> m_scale;
    DataBinding<glm::vec3> m_localXUnitVector;

    glm::mat4 m_modelMatrix;
};
