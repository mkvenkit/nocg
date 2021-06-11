#version 450 core

in vec3 color;
in vec3 norm;

void main()
{
   
    gl_FragColor = vec4(color, 1.0);    
}


