#version 450 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;

uniform mat4 vMat;
uniform mat4 pMat;
uniform mat4 mMat;

out VS_OUT {
	out vec3 N;
	out vec3 L;
	out vec3 V;
	out vec2 tc;
	out vec3 tg;
	out vec3 bn;
} vs_out;

void main()
{
	// vertex in world coords
	vec3 wcVert = (vMat * mMat * vec4(aVert, 1.0)).xyz;
	// normal in world coords
	mat4 nMat = transpose(inverse(vMat * mMat));
	vs_out.N = (nMat* vec4(aNorm, 1.0)).xyz;
	
	// diffuse 
	vec3 lightPos = vec3(0.0, 0.0, 10.0);
    vs_out.L = lightPos - wcVert;

	// specular 
	vs_out.V = -wcVert;

	gl_Position = pMat * vMat * mMat * vec4(aVert, 1.0);

	// tex coord
	vs_out.tc = aTexCoord;

	// tangent 
	vs_out.tg = aTangent;

	// binormal
	vs_out.bn = cross(aNorm, aTangent);

	// compute TBN matrix 
	mat3 matTBN = mat3(aTangent, vs_out.bn, aNorm);


}

