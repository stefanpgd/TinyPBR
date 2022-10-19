#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TextureCoord;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_mr;
uniform sampler2D texture_ao;
uniform sampler2D texture_emissive;

void main()
{
   vec4 diffuseTex = texture(texture_diffuse, TextureCoord);
   vec4 ao = texture(texture_ao, TextureCoord);
   diffuseTex *= ao;
   FragColor = vec4(ao.rgb, 1.0);
}