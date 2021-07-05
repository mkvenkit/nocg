#version 450 core

uniform bool enableRimLight;
uniform sampler2D sampler;

in VS_OUT {
	in vec3 N;
	in vec3 L;
	in vec3 V;
	in vec2 tc;
} fs_in;

out vec4 color;

vec3 getColor()
{
    return clamp(noise3(100.0), 0, 1);
}

void main()
{
	// normalise vectors
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	// ambient 
	vec3 camb = vec3(0.1);

	// texture 
	vec3 texCol = texture(sampler, fs_in.tc).xyz;

	// diffuse 
    float diff = max(dot(N, L), 0.0);
	vec3 Ka = texCol; //getColor();
	float Ia = 0.5;
	vec3 cdiff = diff*Ka*Ia;

	// specular 
	vec3 Ks = vec3(1.0, 1.0, 1.0);
	float Is = 1.0;	
	vec3 R = reflect(-L, N);
	float a = 32.0;
	float spec = pow(max(dot(R, V), 0.0), a);
	vec3 cspec = spec*Ks*Is;

	// final color 
	color = vec4(camb + cdiff + cspec, 1.0);

    //color = vec4(1.0, 0.0, 0.0, 1.0);
}


