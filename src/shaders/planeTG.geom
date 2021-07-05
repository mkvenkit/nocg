#version 450 core

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 5) out;

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

layout(std430, binding = 2) buffer ColorSSBO {
    vec3 colssbo;
};


void main()
{
    vec4 center = vec4(0.0);
    vec3 norm = vec3(0.0);

    const uint ind[5] = uint[5](0, 1, 2, 3, 4);


    for (int i = 0; i < gl_in.length(); i++) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.N = gs_in[i].N;
        gs_out.L = gs_in[i].L;
        gs_out.V = gs_in[i].V;
        gs_out.tc = gs_in[i].tc;
        EmitVertex();

        center = center + gl_in[i].gl_Position;
        norm = norm + gs_in[i].N;
    }

    center = 0.25*center;
    norm = 0.5*norm;

    float dist = 1;
    gl_Position = vec4(center.xyz + dist*norm, center.w);
    EmitVertex();

    EndPrimitive();

    colssbo = vec3(0.1, 0.2, 0.3);
}