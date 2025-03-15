#version 330 core

#define MAX_LIGHTS 8

// Light types
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Light{
    uint type;
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 falloff;
};

uniform Light lights[MAX_LIGHTS];
uniform uint lightCount;

uniform vec3 ambientColor;
uniform vec3 materialColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(materialColor, 1);
}