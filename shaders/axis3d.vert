// axis3d.vert

#version 450 core

layout(location = 0) in vec3 vPos;

uniform mat4 vMat;
uniform mat4 pMat;

void main()
{
	gl_Position = pMat * vMat* vec4(vPos, 1.0);
}

