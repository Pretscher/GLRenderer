
#version 330 core
out vec4 FragColor;

uniform vec3 color = vec3(1.0f);

void main()
{
    FragColor = vec4(1.0f) * vec4(color, 1.0f); // set all 4 vector values to 1.0
}