#version 450 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec3 aNorm;

uniform mat4 vMat;
uniform mat4 pMat;

out vec3 wcVert;
out vec3 color;
out vec3 norm;

void main()
{
	gl_Position = pMat * vMat* vec4(aVert, 1.0);
	color = vec3(1.0, 1.0, 0.0);

	// vertex in world coords
	wcVert = (vMat* vec4(aVert, 1.0)).xyz;

	// normal 
	norm = aNorm;
}

