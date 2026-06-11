#version 330 core

in vec3 thecolor;
out vec4 fragColor;

void main()
{
    fragColor = vec4(thecolor,1.0);
}
