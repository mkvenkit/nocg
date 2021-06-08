#version 450 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;

uniform mat4 vMat;
uniform mat4 pMat;

out vec3 color;

void main()
{
	gl_Position = pMat * vMat* vec4(vPos, 1.0);
	color = vCol;
}

