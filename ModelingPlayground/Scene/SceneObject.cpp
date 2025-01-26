#include "SceneObject.h"
#include "glm/ext/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

SceneObject::SceneObject(std::string name, PrimitiveType primitiveType, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):
    m_name(name),
    m_primitiveType(primitiveType),
    m_position(position),
    m_rotation(rotation),
    m_scale(scale),
    m_modelMatrix(glm::mat4(1))
{
    UpdateModelMatrix();
}

glm::mat4 SceneObject::GetModelMatrix() const
{
    return m_modelMatrix;
}

std::string SceneObject::GetName() const
{
    return m_name;
}

void SceneObject::UpdateModelMatrix()
{
    // scale
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), m_scale);

    // rotation
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(m_rotation.x, m_rotation.y, m_rotation.z);

    // translation
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), m_position);

    // model matrix
    m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}
