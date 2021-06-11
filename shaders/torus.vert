#version 450 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec3 aNorm;

uniform mat4 vMat;
uniform mat4 pMat;

out vec3 color;
out vec3 norm;

void main()
{
	// vertex in world coords
	vec3 wcVert = (vMat* vec4(aVert, 1.0)).xyz;
	// normal in world coords
	mat4 nMat = transpose(inverse(vMat));
	vec3 N = normalize((nMat* vec4(aNorm, 1.0)).xyz);

	// ambient 
	vec3 camb = vec3(0.1);

	// diffuse 
	vec3 lightPos = vec3(10.0, 0.0, 10.0);
    vec3 L = normalize(lightPos - wcVert);
    float diff = max(dot(N, L), 0.0);
	vec3 Ka = vec3(1.0, 1.0, 0.0);
	float Ia = 1.0;
	vec3 cdiff = diff*Ka;

	// final color 
	color = camb + cdiff;

	gl_Position = pMat * vMat* vec4(aVert, 1.0);

	norm = aNorm;	
}

