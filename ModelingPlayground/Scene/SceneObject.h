#pragma once

#include <string>

#include "glm/glm.hpp"

enum class PrimitiveType
{
    Triangle,
};

class SceneObject
{
public:
    SceneObject(std::string name, PrimitiveType primitiveType, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
    glm::mat4 GetModelMatrix() const;
    std::string GetName() const;
    
private:
    void UpdateModelMatrix();

    std::string m_name;
    
    PrimitiveType m_primitiveType;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 m_modelMatrix;
};
