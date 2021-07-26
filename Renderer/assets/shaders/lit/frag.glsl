#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;   
    sampler2D emission;
    float     shininess;
};  

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct SpotLight {    
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};  

//Lights
#define MAX_DIRECTIONAL_LIGHTS 1  
uniform DirLight dirLights[MAX_DIRECTIONAL_LIGHTS];
#define MAX_POINT_LIGHTS 4  
uniform PointLight pointLights[MAX_POINT_LIGHTS];
#define MAX_SPOT_LIGHTS 1
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int dirLightsLength = 0;
uniform int pointLightsLength = 0;
uniform int spotLightsLength = 0;

uniform vec3 viewPos;  
uniform Material material;
uniform float time;

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords; 

//method prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex); 

void main()
{
    //produce textures
    vec3 specularTex = texture(material.specular, TexCoords).rgb;
    vec3 diffuseTex = texture(material.diffuse, TexCoords).rgb;
    vec2 matrixCoords = TexCoords;
    matrixCoords.y += time;
    vec3 emissionTex = texture(material.emission, matrixCoords).rgb;   
    vec3 emission = emissionTex; 
    if(specularTex.r > 0) {
        emission *= 0.0f;
    }

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    // phase 1: Directional lighting
    for(int i = 0; i < dirLightsLength; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir, diffuseTex, specularTex);
    // phase 2: Point lights
    for(int i = 0; i < pointLightsLength; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffuseTex, specularTex);    
    // phase 3: Spot light
    for(int i = 0; i < spotLightsLength; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, diffuseTex, specularTex);
    
    FragColor = vec4(result + emission, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightDirDist = light.position - fragPos;
    vec3 lightDir = normalize(lightDirDist);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(lightDirDist);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightDirDist = light.position - fragPos;
    vec3 lightDir = normalize(lightDirDist);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(lightDirDist);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * spec * specularTex;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
} 

