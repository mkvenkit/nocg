#version 450 core

in VS_OUT {
	in vec3 N;
	in vec3 L;
	in vec3 V;
} fs_in;

void main()
{
	// normalise vectors
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	// ambient 
	vec3 camb = vec3(0.1);

	// diffuse 
    float diff = max(dot(N, L), 0.0);
	vec3 Ka = vec3(1.0, 0.0, 0.0);
	float Ia = 0.5;
	vec3 cdiff = diff*Ka*Ia;

	// specular 
	vec3 Ks = vec3(1.0, 1.0, 1.0);
	float Is = 1.0;
	vec3 R = reflect(-L, N);
	float a = 128.0;
	float spec = pow(max(dot(R, V), 0.0), a);
	vec3 cspec = spec*Ks*Is;

	// final color 
	vec3 color = camb  + cdiff + cspec;
   
    gl_FragColor = vec4(color, 1.0);    
}


