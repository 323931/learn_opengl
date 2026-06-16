#version 330 core

in vec3 theNormal;
in vec3 thePosition;
out vec4 fragColor;

uniform vec3 lightPosition;

void main()
{
    vec3 lightVertex = normalize(lightPosition - thePosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);
    fragColor = vec4(diffuse,diffuse,diffuse,1.0);
}
