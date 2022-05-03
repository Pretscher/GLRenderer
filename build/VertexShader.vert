#version 420 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec3 VertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

void main()
{
    //apply matrices
    mat4 mvp =  projection * view * model;
    gl_Position = mvp * vec4(aPos, 1.0f);

    //pass on to fragment shader
    VertColor = aColor;
    TexCoord = aTexCoord;
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);  
    FragPos = vec3(model * vec4(aPos, 1.0));
}