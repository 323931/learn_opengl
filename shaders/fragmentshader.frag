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
uniform float roughnessValue;
uniform sampler2D roughnessTexture;
uniform bool useRoughnessTexture;
uniform bool debugSpecularOnly;

void main()
{
    float distanceToLight = length(lightPosition - theWorldPosition);
    float attenuation = 1.0 / (lightConstant + lightLinear * distanceToLight + lightQuadratic * distanceToLight * distanceToLight);

    //the diffuse
    vec3 theNormal = normalize(theWorldNormal);
    vec3 lightVertex = normalize(lightPosition - theWorldPosition);
    float diffuse = max(dot(theNormal,lightVertex),0.0);

    // Roughness 0 is smooth with a small concentrated highlight;
    // roughness 1 is rough with a broad soft highlight.
    float roughness = useRoughnessTexture
        ? texture(roughnessTexture, theTexCoord).r
        : roughnessValue;
    float fragmentShininess = mix(256.0, 2.0, clamp(roughness, 0.0, 1.0));

    //the specular
    vec3 reflectLight = normalize(reflect(-lightVertex,theNormal));
    vec3 viewVertex = normalize(viewPositionWorld - theWorldPosition);
    // 反射方向背对观察者时没有镜面高光，同时避免对负数进行幂运算。
    float specular = pow(max(dot(reflectLight,viewVertex),0.0), fragmentShininess);

    vec3 surfaceColor = useDiffuseTexture ? texture(diffuseTexture, theTexCoord).rgb : theVertexColor;
    vec3 ambientColor = surfaceColor * ambientLight;
    vec3 diffuseColor = attenuation * lightColor * surfaceColor * diffuse;
    float specularStrength = mix(1.5, 0.15, clamp(roughness, 0.0, 1.0));
    vec3 specularColor = attenuation * lightColor * specular * specularStrength;
    // Debug view: remove attenuation and brighten tiny values so the highlight shape is easy to inspect.
    vec3 specularDebugColor = vec3(pow(specular, 0.25));

    vec3 color = debugSpecularOnly
        ? specularDebugColor
        : ambientColor + diffuseColor + specularColor;
    fragColor = vec4(color, 1.0);
}
