#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrix0;
uniform mat4 lightMatrix1;
uniform mat4 lightMatrix2;
uniform mat4 lightMatrix3;
uniform mat4 lightMatrix4;
uniform mat4 lightMatrix5;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    mat4 lightMatrices[6] = {
        lightMatrix0,
        lightMatrix1,
        lightMatrix2,
        lightMatrix3,
        lightMatrix4,
        lightMatrix5
    };
    
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  