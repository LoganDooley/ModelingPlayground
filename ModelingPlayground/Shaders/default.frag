﻿#version 330 core

#define MAX_LIGHTS 8

// Light types
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#define PI 3.1415926

struct Light{
    int type;
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 falloff;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 ambientColor;
uniform vec3 materialColor;
uniform float roughness;
uniform float metallic;

uniform vec3 cameraPosition;

in vec3 vertexWorldPosition;
in vec3 vertexNormal;

out vec4 FragColor;

struct LightData {
    vec3 L;
    vec3 H;
    vec3 radiance;
};

LightData getDirectionalLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    lightData.L = -lights[lightIndex].direction;
    lightData.H = normalize(V + lightData.L);
    lightData.radiance = lights[lightIndex].color;
    return lightData;
}

LightData getPointLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    return lightData;
}

LightData getSpotLightData(int lightIndex, vec3 N, vec3 V){
    LightData lightData;
    return lightData;
}

float distributionGGX(float NdotH){
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
    return a2 / max(denom, 0.0001);
}

float geometrySmith(float NdotV, float NdotL){
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
    float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float HdotV, vec3 baseReflectivity){
    return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - HdotV, 5.0);
}

vec3 getLightContribution(int lightIndex, vec3 N, vec3 V, vec3 baseReflectivity){
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
    
    float D = distributionGGX(NdotH);
    float G = geometrySmith(NdotV, NdotL);
    vec3 F = fresnelSchlick(HdotV, baseReflectivity);
    
    vec3 specular = D * G * F;
    specular /= 4.0 * NdotV * NdotL;
    
    vec3 kD = vec3(1.0) - F;
    
    kD *= 1.0 - metallic;
    
    return (kD * materialColor / PI + specular) * lightData.radiance * NdotL;
}

void main()
{
    vec3 normal = normalize(vertexNormal);
    vec3 toCamera = normalize(cameraPosition - vertexWorldPosition);
    
    vec3 baseReflectivity = mix(vec3(0.04), materialColor, metallic);
    
    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i<lightCount; i++){
        Lo += getLightContribution(i, normal, toCamera, baseReflectivity);
    }

    // Add ambient light
    vec3 ambient = ambientColor * materialColor;
    
    vec3 color = ambient + Lo;
    
    // Apply HDR Tonemapping
    color = color / (color + vec3(1.0));
    
    // Apply gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1);
}