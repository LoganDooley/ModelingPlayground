#pragma once

#include "Component.h"
#include "../../Utils/EnumLabels/PrimitiveTypeEnumLabel.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../OpenGLHelpers/OpenGLPrimitiveManager.h"


class PrimitiveComponent : public Component
{
public:
    PrimitiveComponent(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager);

    void SetPrimitiveManager(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager);
    
    void RenderInspector() override;
    std::string GetCustomPrimitiveFilePath() const;
    PrimitiveType GetPrimitiveType() const;

    friend void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent);
    friend void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent);

private:
    std::shared_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
    std::string m_customPrimitiveFilePath;
    PrimitiveType m_primitiveType;
    int m_currentItem;
};
