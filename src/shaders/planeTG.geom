#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 vMat;
uniform mat4 pMat;
uniform mat4 mMat;

// input from vertex shader
in VS_OUT {
	vec3 N;
	vec3 L;
	vec3 V;
	vec2 tc;
} gs_in[];

// output to fragment shader 
out VS_OUT {
	vec3 N;
	vec3 L;
	vec3 V;
	vec2 tc;
} gs_out;

void main()
{
    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.N = gs_in[i].N;
        gs_out.L = gs_in[i].L;
        gs_out.V = gs_in[i].V;
        gs_out.tc = gs_in[i].tc;
        EmitVertex();
    }
    EndPrimitive();
}