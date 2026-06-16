#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in mat4 fullTransformMartix;

uniform vec3 uni_color;
uniform mat4 modelTransformMartix;
uniform mat4 projectionMartix;
uniform vec3 ambientLight;
uniform vec3 lightDirection;

out vec3 theNormal;
out vec3 thePosition;

void main()
{
    vec4 v = vec4(position,1.0);
    gl_Position = fullTransformMartix * v;
    theNormal = normalize(vertexNormal);
    thePosition = position;
}
