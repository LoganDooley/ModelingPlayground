#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat3 inverseTransposeModelMatrix;
uniform mat4 cameraMatrix;

out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec2 vertexTexCoord;

void main()
{
    vec4 vertexWorldPosition4;
    vertexWorldPosition4 = modelMatrix * vec4(aPos, 1.0);
    
    vertexWorldPosition = vec3(vertexWorldPosition4);
    vertexNormal = normalize(inverseTransposeModelMatrix * aNormal);
    vertexTexCoord = aTexCoord;
    gl_Position = cameraMatrix * vertexWorldPosition4;
}  