#version 450 core

uniform bool enableRimLight;
uniform sampler2D sampler;

in VS_OUT {
	in vec3 N;
	in vec3 L;
	in vec3 V;
	in vec2 tc;
	in vec3 tg;
	in vec3 bn;
} fs_in;

out vec4 color;

#define M_PI 3.14159265358979323846f

void main()
{
	// normalise vectors
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	// stripes 
	float val = clamp(round(sin( 20 * fs_in.tc.x * 3.14156)), 0, 1);
	vec3 col = mix(vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), val);

	// ambient 
	vec3 camb = vec3(0.1);

	// diffuse 
    float diff = max(dot(N, L), 0.0);
	vec3 Ka = col;
	float Ia = 0.5;
	vec3 cdiff = diff*Ka*Ia;

	// specular 
	vec3 Ks = vec3(1.0, 1.0, 1.0);
	float Is = 1.0;	
	vec3 R = reflect(-L, N);
	float a = 32.0;
	float spec = pow(max(dot(R, V), 0.0), a);
	vec3 cspec = spec*Ks*Is;

	// rim light 
	vec3 crim = vec3(0.0);

	if (enableRimLight) {
		float rim = (1.0 - dot(N, V));
		rim = smoothstep(0.0, 1.0, rim);
		float rim_exp = 3.5;
		rim = pow(rim, rim_exp);
		vec3 rim_col = vec3(0.1, 0.1, 0.1);
		crim = rim * rim_col;
	}

	color = vec4(camb + cdiff + cspec + crim, 1.0);

	color = vec4(normalize(0.5*(fs_in.tg + 1.0)), 1.0);
}


