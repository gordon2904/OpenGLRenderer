#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    float     shininess;
};  

uniform vec3 viewPos;  
uniform Material material;
uniform float time;

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords; 
in vec3 Tangent;
in vec3 BiTangent; 

void main()
{
    //produce textures
    vec4 specularTex = texture(material.texture_specular1, TexCoords);

    vec4 diffuseTex = texture(material.texture_diffuse1, TexCoords);

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    if(diffuseTex.a < 0.1) {
        discard;
    }
    FragColor = diffuseTex;
}