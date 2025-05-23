﻿#version 460 core

#extension GL_ARB_bindless_texture: require

#define MAX_LIGHTS 50

// Light types
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define PI 3.1415926

// 144 bytes
struct Light{
    sampler2D shadowMap; // 0
    samplerCube cubeShadowMap; // 8
    int type; // 16
    float colorR; // 20
    float colorG; // 24
    float colorB; // 28
    float positionX; // 32
    float positionY; // 36
    float positionZ; // 40
    float directionX; // 44
    float directionY; // 48
    float directionZ; // 52
    float falloffA; // 56
    float falloffB; // 60
    mat4 lightMatrix; // 64
    float falloffC; // 128
    bool hasShadowMap; // 132
};

layout (std140, binding = 0) uniform LightsBlock
{
    Light lights[MAX_LIGHTS]; // lights[i] = 144 * i
    int lightCount; // lightCount = 144 * MAX_LIGHTS
};

uniform vec3 ambientColor = vec3(0.01, 0.01, 0.01);

struct Material{
    bool useMaterialTexture;
    vec4 materialColor;
    sampler2D materialTexture;
    bool useRoughnessMap;
    float roughness;
    sampler2D roughnessMap;
    bool useMetallicMap;
    float metallic;
    sampler2D metallicMap;
};

layout (std430, binding = 5) readonly buffer MaterialBuffer {
    Material materials[];
};

uniform vec3 cameraPosition;

flat in int drawID;
in vec3 vertexWorldPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

out vec4 FragColor;

struct LightData {
    vec3 L;
    vec3 H;
    vec3 radiance;
};

vec4 GetMaterialColorValue(Material material){
    if(!material.useMaterialTexture){
        return material.materialColor;
    }
    return texture(material.materialTexture, vertexTexCoord);
}

float GetRoughnessValue(Material material){
    if(!material.useRoughnessMap){
        return material.roughness;
    }
    return texture(material.roughnessMap, vertexTexCoord).r;
}

float GetMetallicValue(Material material){
    if(!material.useMetallicMap){
        return material.metallic;
    }
    return texture(material.metallicMap, vertexTexCoord).r;
}

float GetOmnidirectionalShadowFactor(int lightIndex){
    vec3 lightPosition = vec3(lights[lightIndex].positionX, lights[lightIndex].positionY, lights[lightIndex].positionZ);
    vec3 fragToLight = vertexWorldPosition - lightPosition;
    float currentDepth = length(fragToLight);
    
    float shadowFactor = 0.0;
    float samples = 4.0;
    float offset = 0.1;
    float increment = offset/(samples * 0.5);
    
    for(float x = -offset; x < offset; x += increment){
        for(float y = -offset; y < offset; y += increment){
            for(float z = -offset; z < offset; z += increment){
                float pcfDepth = texture(lights[lightIndex].cubeShadowMap, fragToLight + vec3(x, y, z)).r;
                pcfDepth *= 100;
                shadowFactor += currentDepth > pcfDepth ? 0.0 : 1.0;
            }
        }
    }
    
    return shadowFactor / (samples * samples * samples);
}

float GetUnidirectionalShadowFactor(int lightIndex){
    vec4 lightSpacePosition = lights[lightIndex].lightMatrix * vec4(vertexWorldPosition, 1);
    vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
    projCoords = projCoords * 0.5 + vec3(0.5);
    float currentDepth = projCoords.z;
    
    float shadowFactor = 0.0;
    vec2 texelSize = 1.0 / textureSize(lights[lightIndex].shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <=1; ++y){
            float pcfDepth = texture(lights[lightIndex].shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowFactor += currentDepth > pcfDepth ? 0.0 : 1.0;
        }
    }
    
    return shadowFactor / 9.0;
}

float GetShadowFactor(int lightIndex){
    if(!lights[lightIndex].hasShadowMap){
        return 1;
    }
    
    if(lights[lightIndex].type == POINT_LIGHT){
        return GetOmnidirectionalShadowFactor(lightIndex);
    }
    else{
        return GetUnidirectionalShadowFactor(lightIndex);
    }
}

LightData getDirectionalLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    Light light = lights[lightIndex];
    vec3 lightDirection = vec3(light.directionX, light.directionY, light.directionZ);
    vec3 lightColor = vec3(light.colorR, light.colorG, light.colorB);
    
    lightData.L = -lightDirection;
    lightData.H = normalize(V + lightData.L);
    lightData.radiance = lightColor;
    return lightData;
}

LightData getPointLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    Light light = lights[lightIndex];
    vec3 lightPosition = vec3(light.positionX, light.positionY, light.positionZ);
    vec3 lightColor = vec3(light.colorR, light.colorG, light.colorB);
    
    vec3 toLight = lightPosition - vertexWorldPosition;
    float d = max(length(toLight), 0.0001);
    lightData.L = toLight/d;
    lightData.H = normalize(V + lightData.L);
    float attenuation = clamp(1.0/(light.falloffA + d * light.falloffB + d * d * light.falloffC), 0.0, 1.0);
    lightData.radiance = lightColor * attenuation;
    return lightData;
}

LightData getSpotLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    Light light = lights[lightIndex];
    vec3 lightPosition = vec3(light.positionX, light.positionY, light.positionZ);
    vec3 lightDirection = vec3(light.directionX, light.directionY, light.directionZ);
    vec3 lightColor = vec3(light.colorR, light.colorG, light.colorB);
    
    vec3 toLight = lightPosition - vertexWorldPosition;
    float d = max(length(toLight), 0.0001);
    lightData.L = toLight/d;
    lightData.H = normalize(V + lightData.L);
    float theta = acos(dot(-lightData.L, lightDirection));
    float innerAngle = radians(light.falloffA);
    float outerAngle = radians(light.falloffB);
    float attenuation = clamp((theta - outerAngle)/(innerAngle - outerAngle), 0.0, 1.0);
    lightData.radiance = lightColor * attenuation;
    return lightData;
}

float distributionGGX(float roughnessValue, float NdotH){
    float a = roughnessValue * roughnessValue;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
    return a2 / max(denom, 0.0001);
}

float geometrySmith(float roughnessValue, float NdotV, float NdotL){
    float r = roughnessValue + 1.0;
    float k = (r * r) / 8.0;
    float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
    float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float HdotV, vec3 baseReflectivity){
    return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - HdotV, 5.0);
}

vec3 getLightContribution(int lightIndex, vec3 N, vec3 V, vec3 baseReflectivity, vec3 materialColorValue, float roughnessValue, float metallicValue){
    float shadowFactor = GetShadowFactor(lightIndex);
    if(shadowFactor == 0){
        return vec3(0);
    }
    
    LightData lightData;
    if(lights[lightIndex].type == DIRECTIONAL_LIGHT){
        lightData = getDirectionalLightData(lightIndex, N, V);
    }
    else if(lights[lightIndex].type == POINT_LIGHT){
        lightData = getPointLightData(lightIndex, N, V);
    }
    else if(lights[lightIndex].type == SPOT_LIGHT){
        lightData = getSpotLightData(lightIndex, N, V);
    }
    
    // Cook-torrance BRDF
    float NdotV = max(dot(N, V), 0.0001);
    float NdotL = max(dot(N, lightData.L), 0.0001);
    float HdotV = max(dot(lightData.H, V), 0.0);
    float NdotH = max(dot(N, lightData.H), 0.0);
    
    float D = distributionGGX(roughnessValue, NdotH);
    float G = geometrySmith(roughnessValue, NdotV, NdotL);
    vec3 F = fresnelSchlick(HdotV, baseReflectivity);
    
    vec3 specular = D * G * F;
    specular /= 4.0 * NdotV * NdotL;
    
    vec3 kD = vec3(1.0) - F;
    
    kD *= 1.0 - metallicValue;
    
    return (kD * materialColorValue / PI + specular) * lightData.radiance * NdotL * shadowFactor;
}

void main()
{
    vec3 normal = normalize(vertexNormal);
    vec3 toCamera = normalize(cameraPosition - vertexWorldPosition);
    
    Material material = materials[drawID];
    
    vec4 materialColorValue = GetMaterialColorValue(material);
    float roughnessValue = GetRoughnessValue(material);
    float metallicValue = GetMetallicValue(material);
    
    vec3 baseReflectivity = mix(vec3(0.04), materialColorValue.xyz, metallicValue);
    
    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i<lightCount; i++){
        Lo += getLightContribution(i, normal, toCamera, baseReflectivity, materialColorValue.xyz, roughnessValue, metallicValue);
    }

    // Add ambient light
    vec3 ambient = ambientColor * materialColorValue.xyz;
    
    vec3 color = ambient + Lo;
    
    // Apply HDR Tonemapping
    color = color / (color + vec3(1.0));
    
    // Apply gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    float transparency = materialColorValue.w;
    
    FragColor = vec4(color, transparency);
}