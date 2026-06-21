#version 330 core

in vec3 theWorldNormal;
in vec3 theWorldPosition;
in vec3 theVertexColor;
out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;
uniform vec3 ambientLight;
uniform vec3 viewPositionWorld;

void main()
{
    float distanceToLight = length(lightPosition - theWorldPosition);
    float attenuation = 1.0 / (lightConstant + lightLinear * distanceToLight + lightQuadratic * distanceToLight * distanceToLight);

    //the diffuse
    vec3 theNormal = normalize(theWorldNormal);
    vec3 lightVertex = normalize(lightPosition - theWorldPosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);

    //the specular
    vec3 reflectLight = normalize(reflect(-lightVertex,theNormal));
    vec3 viewVertex = normalize(viewPositionWorld - theWorldPosition);
    //这里pow函数如果是奇数次幂指数会导致结果为负数，出现的结果就是减去某些颜色，所以要用clamp/max做限制
    float specular = pow(max(dot(reflectLight,viewVertex),0.0), 32.0);

    vec3 color = theVertexColor * ambientLight
        + attenuation * lightColor * (theVertexColor * diffuse + vec3(specular));
    fragColor = vec4(color, 1.0);
}
