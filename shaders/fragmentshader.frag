#version 330 core

in vec3 theWorldNormal;
in vec3 theWorldPosition;
in vec3 theVertexColor;
in vec2 theTexCoord;
out vec4 fragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightConstant;
uniform float lightLinear;
uniform float lightQuadratic;
uniform vec3 ambientLight;
uniform vec3 viewPositionWorld;
uniform sampler2D diffuseTexture;
uniform bool useDiffuseTexture;
uniform float shininess;
uniform sampler2D roughnessTexture;
uniform bool useRoughnessTexture;

void main()
{
    float distanceToLight = length(lightPosition - theWorldPosition);
    float attenuation = 1.0 / (lightConstant + lightLinear * distanceToLight + lightQuadratic * distanceToLight * distanceToLight);

    //the diffuse
    vec3 theNormal = normalize(theWorldNormal);
    vec3 lightVertex = normalize(lightPosition - theWorldPosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);

    // Roughness map: 0 is smooth with a small concentrated highlight;
    // 1 is rough with a broad soft highlight. shininess is the fallback value.
    float roughness = useRoughnessTexture ? texture(roughnessTexture, theTexCoord).r : 0.0;
    float fragmentShininess = useRoughnessTexture ? mix(256.0, 2.0, roughness) : shininess;

    //the specular
    vec3 reflectLight = normalize(reflect(-lightVertex,theNormal));
    vec3 viewVertex = normalize(viewPositionWorld - theWorldPosition);
    // 反射方向背对观察者时没有镜面高光，同时避免对负数进行幂运算。
    float specular = pow(max(dot(reflectLight,viewVertex),0.0), fragmentShininess);

    vec3 surfaceColor = useDiffuseTexture ? texture(diffuseTexture, theTexCoord).rgb : theVertexColor;
    vec3 ambientColor = surfaceColor * ambientLight;
    vec3 diffuseColor = attenuation * lightColor * surfaceColor * diffuse;
    vec3 specularColor = attenuation * lightColor * specular;

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
    //fragColor = vec4(vec3(roughness),1.0);
    //fragColor = vec4(
    //vec3(specular),
    //1.0
    //);
}