#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (binding = 3, std430) readonly buffer ModelMatrixBuffer {
    mat4 modelMatrices[];
};

layout (binding = 4, std430) readonly buffer InverseTransposeModelMatrixBuffer {
    mat4 inverseTransposeModelMatrices[];
};

uniform mat4 cameraMatrix;

out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec2 vertexTexCoord;
out int drawID;

void main()
{
    drawID = gl_DrawID;
    mat4 modelMatrix = modelMatrices[gl_DrawID];
    mat3 inverseTransposeModelMatrix = mat3(inverseTransposeModelMatrices[gl_DrawID]);
    
    vec4 vertexWorldPosition4;
    vertexWorldPosition4 = modelMatrix * vec4(aPos, 1.0);
    
    vertexWorldPosition = vec3(vertexWorldPosition4);
    vertexNormal = normalize(inverseTransposeModelMatrix * aNormal);
    vertexTexCoord = aTexCoord;
    gl_Position = cameraMatrix * vertexWorldPosition4;
}  