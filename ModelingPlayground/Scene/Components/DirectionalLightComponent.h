#pragma once

#include <glm/vec3.hpp>

#include "Component.h"
#include "../../Utils/DataBinding.h"

class DirectionalLightComponent : public Component
{
public:
    DirectionalLightComponent();

    void RenderInspector() override;

    const glm::vec3& GetLightColor() const;
    DataBinding<glm::vec3>& GetLightColorDataBinding();
    
private:
    DataBinding<glm::vec3> m_lightColor;
};
