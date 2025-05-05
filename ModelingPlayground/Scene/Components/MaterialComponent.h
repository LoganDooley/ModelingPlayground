#pragma once

#include <glm/vec4.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../Utils/DataBinding.h"

class RenderingManager;

class MaterialComponent : public Component
{
public:
    MaterialComponent();

    void SetOpenGLRenderer(std::shared_ptr<RenderingManager> renderingManager);

    void RenderInspector() override;

    DataBinding<bool>& GetUseColorTextureDataBinding();
    DataBinding<glm::vec4>& GetMaterialColorDataBinding();
    DataBinding<std::string>& GetMaterialTextureDataBinding();
    void SetMaterialTexture(const std::string& materialTexture, bool setUseColorTexture = true);

    DataBinding<bool>& GetUseMetallicMapDataBinding();
    DataBinding<float>& GetMetallicDataBinding();
    DataBinding<std::string>& GetMetallicMapDataBinding();
    void SetMetallicMap(const std::string& metallicMap, bool setUseMetallicMap = true);

    DataBinding<bool>& GetUseRoughnessMapDataBinding();
    DataBinding<float>& GetRoughnessDataBinding();
    DataBinding<std::string>& GetRoughnessMapDataBinding();
    void SetRoughnessMap(const std::string& roughnessMap, bool setUseRoughnessMap = true);

    friend void to_json(nlohmann::json& json, const MaterialComponent& materialComponent);
    friend void from_json(const nlohmann::json& json, MaterialComponent& materialComponent);

private:
    std::shared_ptr<RenderingManager> m_renderingManager;

    DataBinding<bool> m_useColorTexture;
    DataBinding<glm::vec4> m_materialColor;
    DataBinding<std::string> m_materialColorTexturePath;
    DataBinding<bool> m_useMetallicMap;
    DataBinding<float> m_metallic;
    DataBinding<std::string> m_metallicMapPath;
    DataBinding<bool> m_useRoughnessMap;
    DataBinding<float> m_roughness;
    DataBinding<std::string> m_roughnessMapPath;
};
