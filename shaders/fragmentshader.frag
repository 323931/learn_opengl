#version 330 core

in vec3 theWorldNormal;
in vec3 theWorldPosition;
out vec4 fragColor;

uniform vec3 lightPosition;

void main()
{
    vec3 theNormal = normalize(theWorldNormal);
    vec3 lightVertex = normalize(lightPosition - theWorldPosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);
    fragColor = vec4(diffuse,diffuse,diffuse,1.0);
}
