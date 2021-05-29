// hello.fs
// simple fragment shader demo 

#version 410 core

uniform vec2 iRes;

out vec4 fragColor;

#define c1 vec4(1.0, 1.0, 1.0, 1.0);
#define c2 vec4(0.0, 0.0, 0.0, 1.0);
#define M_PI 3.1415926535897932384626433832795

void main() {

  vec2 uv = (gl_FragCoord.xy - 0.5*iRes.xy) / iRes.y;

  //if (sin(10*M_PI*length(uv)) > 0.25) {
  if ((uv.x > 0.5) || (uv.y > 0.5)) {
    fragColor = c1;
  }
  else {
    fragColor = c2;
  }
}
