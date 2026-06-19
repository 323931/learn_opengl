#version 330 core

in vec3 theWorldNormal;
in vec3 theWorldPosition;
out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 ambientLight;
uniform vec3 viewPositionWorld;

void main()
{
    //the diffuse
    vec3 theNormal = normalize(theWorldNormal);
    vec3 lightVertex = normalize(lightPosition - theWorldPosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);

    //the specular
    vec3 reflectLight = normalize(reflect(-lightVertex,theNormal));
    vec3 viewVertex = normalize(viewPositionWorld - theWorldPosition);
    float specular = pow(max(dot(reflectLight,viewVertex),0.0), 32.0);

    vec3 color = ambientLight + vec3(diffuse) + vec3(specular);
    fragColor = vec4(color, 1.0);
}
