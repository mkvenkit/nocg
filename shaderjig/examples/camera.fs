// camera.fs

#version 410 core

uniform vec2 iRes;
uniform float iTime;
out vec4 fragColor;

// ray marching constants 
#define MAX_STEPS 100
#define MAX_DIST 100
#define SURF_DIST 0.01

#define M_PI 3.1415926535897932384626433832795

// struct to store ray marching hits 
struct Surf
{
    float d;  // distance
    vec3 c; // color
};

float sdCone( in vec3 p, in vec2 c, float h )
{
  // c is the sin/cos of the angle, h is height
  // Alternatively pass q instead of (c,h),
  // which is the point at the base in 2D
  vec2 q = h*vec2(c.x/c.y,-1.0);
    
  vec2 w = vec2( length(p.xz), p.y );
  vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
  vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
  float k = sign( q.y );
  float d = min(dot( a, a ),dot(b, b));
  float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
  return sqrt(d)*sign(s);
}

float sdCappedCylinder( vec3 p, float h, float r )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(h,r);
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

// SDF for torus 
float sdfTorus(vec3 p, vec2 r)
{
  vec2 q = vec2(p.y, length(p.xz) - r.x);
  float d = length(q) - r.y;
  return d;
}

// SDF for infinite cylinder 
float sdCylinderY( vec3 p, vec3 c )
{
  return length(p.xz-c.xy)-c.z;
}

// SDF for infinite cylinder 
float sdCylinderZ( vec3 p, vec3 c )
{
  return length(p.xy-c.xy)-c.z;
}

// SDF for infinite cylinder 
float sdCylinderX( vec3 p, vec3 c )
{
  return length(p.yz-c.xy)-c.z;
}

// min based on distance
Surf minSurf(Surf s1, Surf s2)
{
    if (s1.d < s2.d) {
        return s1;
    }
    else {
        return s2;
    }
}

Surf getDist(vec3 p)
{
  // sphere 
  float d = sdSphere(p, 0.75);
  Surf sph = Surf(d, vec3(1.0, 0.0, 0.0));

  // cone 
  // (sin(t), cos(t))
  vec2 c = vec2(0.49999999999999994, 0.8660254037844387);
  d = sdCone(p, c, 1.5);
  Surf cone = Surf(d, vec3(0.0, 1.0, 0.0));

  // cylinder 
  d = sdCappedCylinder(p, 0.1, 1.75);
  Surf cyl = Surf(d, vec3(1.0, 1.0, 0.0));

  // torus
  d = sdfTorus(p, vec2(1, 0.2));
  Surf torus = Surf(d, vec3(0.0, 1.0, 1.0));

  // axis cylinders
  float axw = 0.025; // line thickness
  // Z axis 
  d = sdCylinderZ(p, vec3(0, 0, axw));
  Surf cylZ = Surf(d, vec3(0.0, 0.0, 1.0)); 
  // Y axis
  d = sdCylinderY(p, vec3(0, 0, axw));
  Surf cylY = Surf(d, vec3(0.0, 1.0, 0.0)); 
  // X axis
  d = sdCylinderX(p, vec3(0, 0, axw));
  Surf cylX = Surf(d, vec3(1.0, 0.0, 0.0));

  // return min 
  //return dist; 
  return minSurf(minSurf(minSurf(minSurf(minSurf(minSurf(cyl, 
    cone), torus), sph), cylZ), cylY), cylX);
}

Surf rayMarch(vec3 ro, vec3 rd)
{
  Surf hit;
  float d0 = 0.0;

  for (int i = 0; i < MAX_STEPS; i++) {
    vec3 p = ro + d0*rd;
    hit = getDist(p);
    float ds = hit.d;
    d0 += ds;
    if (d0 > MAX_DIST || ds < SURF_DIST) 
      break;
  }

  hit.d = d0;
  return hit;    
}

/* 
  getNormal()

  Computes surface normal using the gradient formula.
*/
vec3 getNormal(vec3 p)
{
  // get distance 
  float d = getDist(p).d;
  
  // define epsilion
  vec2 e = vec2(0.01, 0.0);

  vec3 n = vec3 (getDist(p + e.xyy).d - getDist(p - e.xyy).d,
                 getDist(p + e.yxy).d - getDist(p - e.yxy).d,
                 getDist(p + e.yyx).d - getDist(p - e.yyx).d);
  // normalize vector 
  return normalize(n);
}

mat4 getViewMatrix(vec3 c, vec3 at, vec3 u)
{
    vec3 dir = normalize(c - at);
    vec3 side = normalize(cross(u, dir));
    vec3 up = normalize(cross(dir, side));

    mat4 T = mat4 (
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(c.x, c.y, c.z, 1.0)
    );

    mat4 R = mat4 (
        vec4(side.x, up.x, dir.x, 0.0),
        vec4(side.y, up.y, dir.y, 0.0),
        vec4(side.z, up.z, dir.z, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    return T * transpose(R);
}

float getLight(vec3 p)
{
  // light position
  vec3 lightPos = vec3(5, 5, 5);

  // animate light pos
  vec2 lr = 5.0 * vec2(sin(iTime), cos(iTime));
  lightPos.xz += lr;

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

// camera FOV constants

void main() 
{
    // --------------------
    // camera setup:
    // --------------------

    // width of window in world coordinates 
    float W = 2.0;
    // In world coordinates, uv is on the XY plane 
    // centered at the origin, 
    // with range (-W/2, -H/2) to (W/2, H/2)
    // Where H = W * (iRes.y/iRes.x) 
    vec2 uv = W*(gl_FragCoord.xy - 0.5*iRes.xy) / iRes.y;
    
    
    // set FOV
    float theta_deg = 10;
    float theta = theta_deg * M_PI / 360.0;
    // calculate eye distance from screen
    //float eye_dist = W / (2 * tan(theta));    

    // --------------------
    // ray marching:
    // --------------------
    // set ray origin 
    vec3 eye = vec3(4, 4, 4);
    // looking at?
    vec3 at = vec3(0, 0, 0);
    // look direction 
    vec3 dir = normalize(at - eye);
    // set up vector 
    vec3 up = vec3(0, 0, 1);

    // set distance from eye to screen center
    float c_dist = 2;
    vec3 center = eye + c_dist * dir;    
    
    // get transformation matrix 
    mat4 M = getViewMatrix(center, at, up);
    // transform center of screen to world space
    vec3 cs = (M * vec4(uv.x, uv.y, 0, 1)).xyz;
    
    // ray direction 
    vec3 ray_dir = normalize(cs - eye); 
    // use ray marching get distance to closest object
    Surf hit = rayMarch(eye, ray_dir);

    float d = hit.d;
    // calculate the point on the surface
    vec3 p = eye + d * ray_dir;
    
    // compute diffuse lighting
    float dif = getLight(p);
    
    // color based on diffuse component
    vec3 col = dif * hit.c;

    // set output color
    fragColor = vec4(col, 1.0);
}
