// simple_scene.fs

#version 410 core

uniform vec2 iRes;
uniform float iTime;
out vec4 fragColor;

#define MAX_STEPS 100
#define MAX_DIST 100
#define SURF_DIST 0.01

#define M_PI 3.1415926535897932384626433832795

mat4 LookAt(vec3 eye, vec3 at, vec3 up)
{
  vec3 Z = normalize(at - eye);    
  vec3 X = normalize(cross(Z, up));
  vec3 Y = cross(X, Z);

  Z.xyz = -Z.xyz;

  mat4 viewMatrix0 = mat4(
    vec4(X.x, X.y, X.z, -dot(X, eye)),
    vec4(Y.x, Y.y, Y.z, -dot(Y, eye)),
    vec4(Z.x, Z.y, Z.z, -dot(Z, eye)),
    vec4(0, 0, 0, 1)
  );

  mat4 viewMatrix = mat4(
    vec4(X.x, Y.x, Z.x, 0.0),
    vec4(X.y, Y.y, Z.y, 0.0),
    vec4(X.z, Y.z, Z.z, 0.0),
    vec4(-dot(X, eye), -dot(Y, eye), -dot(Z, eye), 1.0)
  );

  mat4 T_inv = mat4(
    vec4(0.0, 0.0, 0.0, 0.0),
    vec4(0.0, 0.0, 0.0, 0.0),
    vec4(0.0, 0.0, 0.0, 0.0),
    vec4(eye.x, eye.y, eye.z, 1.0)
  );

  mat4 R_inv = mat4(
    vec4(X.x, X.y, X.z, 0.0),
    vec4(Y.x, Y.y, Y.z, 0.0),
    vec4(Z.x, Z.y, Z.z, 0.0),
    vec4(0, 0, 0, 1)
  );

  viewMatrix = T_inv * R_inv;

  return viewMatrix;
}

mat4 g_vM;

// SDF for torus 
float sdfTorus(vec3 p, vec2 r)
{
  vec2 q = vec2(p.y, length(p.xz) - r.x);
  float d = length(q) - r.y;
  return d;
}

float sdPlane( vec3 p, vec3 n, float h )
{
  // n must be normalized
  return dot(p,n) + h ;
}

float getDist(vec3 p)
{
  // distance to plane
  float pd = sdPlane(p, vec3(0, 1, 0), 0);

  
  // rotational transform
  float a = M_PI / 16.0;
  mat4 rotX =  mat4(
		vec4(1.0, 0.0,          0.0,         0.0),
		vec4(0.0, cos(a),  sin(a), 0.0),
	  vec4(0.0, -sin(a), cos(a), 0.0),
	  vec4(0.0, 0.0,          0.0,         1.0)
	);
  
  mat4 rotY =  mat4(
		vec4(cos(a), 0.0,  -sin(a),         0.0),
		vec4(0.0,    1.0,  0.0,             0.0),
	  vec4(sin(a), 0.0,  cos(a),          0.0),
	  vec4(0.0,    0.0,  0.0,             1.0)
	);

  mat4 rotZ =  mat4(
		vec4(cos(a),  sin(a),  0.0,         0.0),
		vec4(-sin(a), cos(a),  0.0,         0.0),
	  vec4(0.0,     0.0,     1.0,         0.0),
	  vec4(0.0,     0.0,     0.0,         1.0)
	);

  //p = (rotX * vec4(p, 1)).xyz;
  //p = (g_vM * vec4(p, 1)).xyz;
  vec2 r = vec2(1, 0.25);
  float td = sdfTorus(p, r);

  // return min 
  return td; //min(pd, td);
}

float rayMarch(vec3 ro, vec3 rd)
{
  float d0 = 0.0;

  for (int i = 0; i < MAX_STEPS; i++) {
    vec3 p = ro + d0*rd;
    float ds = getDist(p);
    d0 += ds;
    if (d0 > MAX_DIST || ds < SURF_DIST)
      break;
  }

  return d0;    
}

/* 
  getNormal()

  Computes surface normal using the gradient formula.
*/
vec3 getNormal(vec3 p)
{
  // get distance 
  float d = getDist(p);
  
  // define epsilion
  vec2 e = vec2(0.01, 0.0);

  vec3 n = vec3 (getDist(p + e.xyy) - getDist(p - e.xyy),
                 getDist(p + e.yxy) - getDist(p - e.yxy),
                 getDist(p + e.yyx) - getDist(p - e.yyx));
  // normalize vector 
  return normalize(n);
}

float getLight(vec3 p)
{
  // light position
  vec3 lightPos = vec3(5, 5, 5);

  // animate light pos
  vec2 lr = 2.0 * vec2(sin(iTime), cos(iTime));
  //lightPos.xz += lr;

  vec3 l = normalize(lightPos - p);
  vec3 n = getNormal(p);
  // clamp to avoid negative values 
  float dif = clamp(dot(l, n), 0.0, 1.0);

  /*
  // add shadow 
  // start marching a bit high above the surface 
  float d = rayMarch(p + n*SURF_DIST*2, l);
  if (d < length(lightPos - p)) {
    dif *= 0.1;
  }*/

  return dif;
}

void main() {
  // Get screen coordinates 
  // Origin at center of screen, range is [-1, 1]
  vec2 uv = 5.0*(gl_FragCoord.xy - 0.5*iRes.xy) / iRes.y;
  
  // get lookat matrix
  vec3 eye = vec3(0, 4.9, 0);
  vec3 at = vec3(0, 0, 0);
  vec3 up = vec3(0, 0, 1);
  g_vM = LookAt(eye, at, up);

  // set up ray 
  vec3 ro = vec3(0, 5, 0);     // ray origin 
  // pt on the screen positioned at the XY plane at Z=0
  vec3 ps = vec3(uv.x, 0.0, uv.y);  
  //ps = (g_vM * vec4(ps.xyz, 1.0)).xyz;
  vec3 rd = normalize(ps - ro); // ray direction 


  float d = rayMarch(ro, rd);

  vec3 p = ro + d * rd;
  
  float dif = getLight(p);
  
  d /= 16.0;

  // enable to debug normals
  // vec3 col = getNormal(p);

  vec3 col = dif * vec3(1, 1, 1.0);
  fragColor = vec4(col, 1.0);

}
