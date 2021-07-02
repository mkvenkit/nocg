#version 450 core

layout (triangles) in;
layout (points, max_vertices = 3) out;

void main()
{
    int i;
    for (i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}