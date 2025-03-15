#version 330 core

uniform vec3 ambientColor;
uniform vec3 materialColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(materialColor, 1);
}