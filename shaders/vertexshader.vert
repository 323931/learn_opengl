#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in mat4 fullTransformMartix;

uniform vec3 uni_color;
uniform mat4 modelTransformMartix;
uniform mat4 projectionMartix;

out vec3 thecolor;

void main()
{
    vec4 v = vec4(position,1.0);
    gl_Position = fullTransformMartix * v;
    thecolor = vertexColor;
}
