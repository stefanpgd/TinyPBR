#version 460 core
in vec3 Normal;
in vec2 TextureCoord;
in vec3 WorldPosition;

uniform vec3 Color;
uniform vec3 CameraPosition;

out vec4 FragColor;

void main()
{
   FragColor = vec4(Color, 1.0);
}