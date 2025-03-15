#pragma once

#include "glm/glm.hpp"

#include "Component.h"

class TransformComponent : public Component
{
public:
    TransformComponent();
    
    void RenderInspector() override;

    const glm::mat4& GetModelMatrix() const;
    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    const glm::vec3& GetScale() const;
    const glm::vec3& GetLocalXUnitVector() const;
    void SetPosition(glm::vec3 newPosition);
    void SetRotation(glm::vec3 newRotation);
    void SetScale(glm::vec3 newScale);
    
private:
    void UpdateModelMatrix();
    void UpdateLocalXUnitVector();

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_localXUnitVector;

    glm::mat4 m_modelMatrix;
};
