#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;

void main()
{
    gl_Position = cameraMatrix * modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}  