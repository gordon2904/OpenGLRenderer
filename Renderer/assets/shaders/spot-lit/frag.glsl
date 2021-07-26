#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;   
    sampler2D emission;
    float     shininess;
};  
struct Light {
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

uniform vec3 viewPos;  
uniform Material material;
uniform Light light;
uniform float time;

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords; 

void main()
{
    //calculate light attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		        light.quadratic * (distance * distance));   

    vec3 specularTex = texture(material.specular, TexCoords).rgb;
    vec3 diffuseTex = texture(material.diffuse, TexCoords).rgb;
    vec2 matrixCoords = TexCoords;
    matrixCoords.y += time;
    vec3 emissionTex = texture(material.emission, matrixCoords).rgb;

    // ambient
    vec3 ambient = light.ambient * diffuseTex;

    // emission
    vec3 emission = emissionTex;
    if(specularTex.r > 0) {
        emission *= 0.0f;
    }

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float theta     = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    

    if(intensity > 0) 
    {       
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * diffuseTex * intensity;  
    
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * specularTex * intensity;  
        
        vec3 result = ((ambient + diffuse + specular) * attenuation) + emission;
        FragColor = vec4(result, 1.0);
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight. 
    {
        vec3 result = ambient + emission;
        FragColor = vec4(result, 1.0);
    }
}