#version 450 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

uniform mat4 vMat;
uniform mat4 pMat;
uniform mat4 mMat;

out VS_OUT {
	out vec3 L;
	out vec3 V;
	out vec2 tc;
} vs_out;

void main()
{
	gl_Position = pMat * vMat * mMat * vec4(aVert, 1.0);

	// tex coord
	vs_out.tc = aTexCoord;

	// binormal
	vec3 bn = cross(aNorm, aTangent);

	// compute TBN matrix 
	mat3 matTBN = mat3(aTangent, bn, aNorm);

	// vertex in world coords
	vec3 wcVert = (vMat * mMat * vec4(aVert, 1.0)).xyz;
	// eye vector 
	vec3 V = -wcVert;
	// transform to tangent space 
	vs_out.V = matTBN * V;

	// light position 	
	vec3 lightPos = vec3(0.0, 0.0, 10.0);
	vec3 L = lightPos - wcVert;
	// transform to tangent space 
	vs_out.L = matTBN * L;
}

