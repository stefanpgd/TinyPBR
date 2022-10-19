#version 460 core
out vec4 FragColor;
in vec3 Normal;

void main()
{
   vec3 normalColor = (Normal + 1.0) * 0.5;
   FragColor = vec4(normalColor, 1.0);
}