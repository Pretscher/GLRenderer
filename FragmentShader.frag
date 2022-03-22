#version 330 core
out vec4 FragColor;
  
//all those vars are passed from the vertex shader
in vec3 VertColor;
in vec2 TexCoord;//to determine how the texture is laid out on the mesh
in vec3 Normal;//for lighting calculations
in vec3 FragPos;//also for lighting calculations

//texture uniforms-------------------------------------------------------------------------------------------------------------------
uniform sampler2D texture0;
uniform float weight0 = 1.0f;//if you use only one texture there is no need to set the tex weights
uniform sampler2D texture1;
uniform float weight1 = 0.0f;//if you use multiple colors this will become quite shiny, so set the weights then
uniform sampler2D texture2;
uniform float weight2 = 0.0f;
uniform sampler2D texture3;
uniform float weight3 = 0.0f;
uniform sampler2D texture4;
uniform float weight4 = 0.0f;

//lighting stuff---------------------------------------------------------------------------------------------------------------------
uniform vec3 viewPos;//camera position, used for specular lighting

struct PointLight {
    vec3 position;
    //attentuation(fading out), first fast, linear decline (linear > quadratic), then slower (quadratic > linear), then slowly becoming 0
    float constant;
    float linear;
    float quadratic;

    //general light properties
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
};
struct DirectionalLight {
    vec3 direction;

    //general light properties
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
};
struct SpotLight {
    vec3  position;
    vec3  direction;

    float innerCutOff;
    float outerCutOff;

    //general light properties
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
};
uniform PointLight pLights[32];
uniform DirectionalLight dLights[32];
uniform SpotLight sLights[32];

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

void calculatePointLights() {
    for(int i = 0; i < pLights.length(); i++) {
        //if light is black (default) ignore it
        if(pLights[i].color.x != 0.0f || pLights[i].color.y != 0.0f || pLights[i].color.z != 0.0f) {
            //attentuation
            float distance = length(pLights[i].position - FragPos);
            float attenuation = 1.0 / (pLights[i].constant + pLights[i].linear * distance + 
    		                    pLights[i].quadratic * (distance * distance));
            
            //we use the same values for diffuse and ambient in our diffuse map
            //ambient lighting
            vec3 ambient = pLights[i].color * pLights[i].ambientIntensity * vec3(texture(material.diffuse, TexCoord));
            ambient *= attenuation;
            FragColor += vec4(ambient, 1.0f);

            //diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pLights[i].position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = pLights[i].color * diff * pLights[i].diffuseIntensity * vec3(texture(material.diffuse, TexCoord));
            diffuse *= attenuation;
            FragColor += vec4(diffuse, 1.0);

            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                                                                                   //Specular map
            vec3 specular = pLights[i].color * (spec * pLights[i].specularIntensity * vec3(texture(material.specular, TexCoord)));  
            specular *= attenuation;
            FragColor += vec4(specular, 1.0);
        }
    }
}

void calculateDirectionalLights() {
    for(int i = 0; i < dLights.length(); i++) {
        if(dLights[i].color.x != 0.0f || dLights[i].color.y != 0.0f || dLights[i].color.z != 0.0f) {
            //we use the same values for diffuse and ambient in our diffuse map
            vec3 ambient = dLights[i].color * dLights[i].ambientIntensity * vec3(texture(material.diffuse, TexCoord));
            //ambient lighting
            FragColor += vec4(ambient, 1.0f);

            //diffuse lighting
            vec3 norm = normalize(Normal);
            //only change compared to pointLight--------------------------------------------------------
            vec3 lightDir = normalize(-dLights[i].direction);

            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = dLights[i].color * diff * dLights[i].diffuseIntensity * vec3(texture(material.diffuse, TexCoord));
            FragColor += vec4(diffuse, 1.0);

            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                                                                                   //Specular map
            vec3 specular = dLights[i].color * (spec * dLights[i].specularIntensity * vec3(texture(material.specular, TexCoord)));  
            FragColor += vec4(specular, 1.0);
        }
    }
}

void calculateSpotLights() {
for(int i = 0; i < sLights.length(); i++) {
        if(sLights[i].color.x != 0.0f || sLights[i].color.y != 0.0f || sLights[i].color.z != 0.0f) {
            //we use the same values for diffuse and ambient in our diffuse map
            vec3 ambient = sLights[i].color * sLights[i].ambientIntensity * vec3(texture(material.diffuse, TexCoord));
            //ambient lighting
            FragColor += vec4(ambient, 1.0f);

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(sLights[i].position - FragPos);

           
            float theta     = dot(lightDir, normalize(-sLights[i].direction));
            float epsilon   = sLights[i].innerCutOff - sLights[i].outerCutOff;
            float spotLightIntensity = clamp((theta - sLights[i].outerCutOff) / epsilon, 0.0, 1.0);   
            //only apply diffuse and specular lighting if fragment is in cone<
            //diffuse lighting
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = sLights[i].color * diff * sLights[i].diffuseIntensity * vec3(texture(material.diffuse, TexCoord)); 
            diffuse *= spotLightIntensity;
            FragColor += vec4(diffuse, 1.0);
             
            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                                                                                       //Specular map
            vec3 specular = sLights[i].color * (spec * sLights[i].specularIntensity * vec3(texture(material.specular, TexCoord)));  
            specular *= spotLightIntensity;
            FragColor += vec4(specular, 1.0);
        }
    }
}

void applyTextures() {
    //(only if the first weight has been passed, colors should not be made black when no texture is bassed)
    FragColor *= 
        texture2D(texture0, TexCoord) * weight0 +
        texture2D(texture1, TexCoord) * weight1 +
        texture2D(texture2, TexCoord) * weight2 +
        texture2D(texture3, TexCoord) * weight3 +
        texture2D(texture4, TexCoord) * weight4;
}

void main()
{
    //lighting effects---------------------------------------------------------------------------------
    calculatePointLights();
    calculateDirectionalLights();
    calculateSpotLights();
    //vertex colors-------------------------------------------------------------------------------------
    //if there is color input, apply it (cannot set default for in-vector so we need to do it like that)
    if(VertColor.x != 0.0f || VertColor.y != 0.0f || VertColor.z != 0.0f) {
        FragColor = vec4(VertColor, 1.0f);
    }
    
    //textures-------------------------------------------------------------------------------------------
    applyTextures();
}