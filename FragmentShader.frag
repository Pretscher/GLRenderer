#version 330 core

out vec4 FragColor;
  
//all those vars are passed from the vertex shader
in vec3 VertColor;
in vec2 TexCoord;//to determine how the texture is laid out on the mesh
in vec3 Normal;//for lighting calculations
in vec3 FragPos;//also for lighting calculations

//texture uniforms-------------------------------------------------------------------------------------------------------------------


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
uniform DirectionalLight dLights[8];
uniform SpotLight sLights[8];

struct Material {
    sampler2D diffuse[8];//renember to set sampler2D values with setInt
    int diffuseMapCount;

    sampler2D specular[8];
    int specMapCount;
}; 
uniform Material material;

//cant be in struct because they have default values
uniform float materialDissuseSensitivity = 1.0f;
uniform float materialSpecularSensitivity = 1.0f;
uniform float materialShininess = 64.0f;

uniform vec3 baseColor = vec3(0.0f);
uniform bool influencedByLighting = true;

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
            vec3 ambient = pLights[i].color * pLights[i].ambientIntensity * materialDissuseSensitivity;
            for(int j = 0; j < material.diffuseMapCount; j++) {
                ambient *= vec3(texture(material.diffuse[j], TexCoord));
            }
           // vec3 ambient = pLights[i].color * pLights[i].ambientIntensity * vec3(texture(material.diffuse, TexCoord));

            ambient *= attenuation;


            //diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pLights[i].position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = pLights[i].color * diff * pLights[i].diffuseIntensity * materialDissuseSensitivity;
            for(int j = 0; j < material.diffuseMapCount; j++) {
                diffuse *= vec3(texture(material.diffuse[j], TexCoord));
            }
          //  vec3 diffuse = pLights[i].color * diff * pLights[i].diffuseIntensity * vec3(texture(material.diffuse, TexCoord));
            diffuse *= attenuation;
           

            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
            vec3 specular = pLights[i].color * spec * pLights[i].specularIntensity * materialSpecularSensitivity;;
            specular *= attenuation;
            for(int j = 0; j < material.specMapCount; j++) {
                specular *= vec3(texture(material.specular[j], TexCoord));
            }
            // vec3 specular = pLights[i].color * (spec * pLights[i].specularIntensity * vec3(texture(material.specular, TexCoord)));  
            FragColor += vec4(ambient, 1.0f);
            FragColor += vec4(diffuse, 1.0f);
            FragColor += vec4(specular, 1.0f);
        }
    }
}

void calculateDirectionalLights() {
    for(int i = 0; i < dLights.length(); i++) {
        if(dLights[i].color.x != 0.0f || dLights[i].color.y != 0.0f || dLights[i].color.z != 0.0f) {
            //ambient lighting
            vec3 ambient = dLights[i].color * dLights[i].ambientIntensity * materialDissuseSensitivity;
            for(int j = 0; j < material.diffuseMapCount; j++) {
                ambient *= vec3(texture(material.diffuse[j], TexCoord));
            }

            //diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(-dLights[i].direction);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = dLights[i].color * diff * dLights[i].diffuseIntensity * materialDissuseSensitivity;
            for(int j = 0; j < material.diffuseMapCount; j++) {
                diffuse *= vec3(texture(material.diffuse[j], TexCoord));
            }

            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
            vec3 specular = dLights[i].color * spec * dLights[i].specularIntensity * materialSpecularSensitivity;;
            for(int j = 0; j < material.specMapCount; j++) {
                specular *= vec3(texture(material.specular[j], TexCoord));
            }
            FragColor += vec4(ambient, 1.0f);
            FragColor += vec4(diffuse, 1.0f);
            FragColor += vec4(specular, 1.0f);
        }
    }
}

void calculateSpotLights() {
for(int i = 0; i < sLights.length(); i++) {
        if(sLights[i].color.x != 0.0f || sLights[i].color.y != 0.0f || sLights[i].color.z != 0.0f) {
            //we use the same values for diffuse and ambient in our diffuse map
            vec3 ambient = pLights[i].color * pLights[i].ambientIntensity * materialDissuseSensitivity;
            for(int j = 0; j < material.diffuseMapCount; j++) {
                ambient *= vec3(texture(material.diffuse[j], TexCoord));
            }

            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(sLights[i].position - FragPos);

           
            float theta     = dot(lightDir, normalize(-sLights[i].direction));
            float epsilon   = sLights[i].innerCutOff - sLights[i].outerCutOff;
            float spotLightIntensity = clamp((theta - sLights[i].outerCutOff) / epsilon, 0.0, 1.0);   
            //only apply diffuse and specular lighting if fragment is in cone<
            //diffuse lighting
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = sLights[i].color * diff * sLights[i].diffuseIntensity * materialDissuseSensitivity; 
            for(int j = 0; j < material.diffuseMapCount; j++) {
                diffuse *= vec3(texture(material.diffuse[j], TexCoord));
            }
            diffuse *= spotLightIntensity;

            //specular lighting
            vec3 viewDir = normalize(viewPos - FragPos);//view vector from viewpos to fragpos
            vec3 reflectDir = reflect(-lightDir, norm);//reflection vector of light
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
            
            vec3 specular = sLights[i].color * (spec * sLights[i].specularIntensity) * materialSpecularSensitivity;  
            for(int j = 0; j < material.specMapCount; j++) {
               specular *= vec3(texture(material.specular[j], TexCoord));
            }
            specular *= spotLightIntensity;

            FragColor += vec4(ambient, 1.0f);
            FragColor += vec4(diffuse, 1.0f);
            FragColor += vec4(specular, 1.0f);
        }
    }
}


void applyVertexColors() {
//if there is color input, apply it (cannot set default for in-vector so we need to do it like that)
    if(VertColor.x != 0.0f || VertColor.y != 0.0f || VertColor.z != 0.0f) {
       FragColor *= vec4(VertColor, 1.0f);
    }
}


void main()
{
    FragColor = vec4(baseColor, 1.0f);

    //lighting effects---------------------------------------------------------------------------------
    if (influencedByLighting == true) {
           
        calculatePointLights();
        calculateDirectionalLights();
        calculateSpotLights();  
    }
    else {
       for (int j = 0; j < material.diffuseMapCount; j++) {
           FragColor *= vec4(vec3(texture(material.diffuse[j], TexCoord)), 1.0f);
       }
    }   
    //vertex colors-------------------------------------------------------------------------------------

    applyVertexColors();
}