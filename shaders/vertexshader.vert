#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in mat4 modelMatrix;

uniform vec3 uni_color;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 theWorldNormal;
out vec3 theWorldPosition;

void main()
{
    vec4 v = vec4(position,1.0);
    mat4 fullTransformMatrix = projectionMatrix *  viewMatrix * modelMatrix;
    gl_Position = fullTransformMatrix * v;
    theWorldNormal = normalize(mat3(transpose(inverse(modelMatrix))) * vertexNormal);
    theWorldPosition = vec3(modelMatrix * v);
}
