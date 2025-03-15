#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat3 inverseTransposeModelMatrix;
uniform mat4 cameraMatrix;

out vec3 vertexNormal;

void main()
{
    gl_Position = cameraMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexNormal = normalize(inverseTransposeModelMatrix * aNormal);
}  