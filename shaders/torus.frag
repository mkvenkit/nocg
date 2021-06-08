#version 450 core

in vec3 color;
in vec3 wcVert;
in vec3 norm;

void main()
{
    vec3 lightPos = vec3(10.0, 10.0, 10.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 lightDir = normalize(lightPos - wcVert);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;
    float ambient = 0.1;
    vec3 result = (ambient + diffuse) * color;
    gl_FragColor = vec4(result, 1.0);
    
    //gl_FragColor = vec4(color, 1.0);
}


