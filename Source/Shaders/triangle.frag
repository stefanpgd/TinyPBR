#version 460 core
in vec3 Normal;
in vec2 TextureCoord;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_mr;
uniform sampler2D texture_ao;
uniform sampler2D texture_emissive;

out vec4 FragColor;

void main()
{
   vec4 diffuseTex = texture(texture_diffuse, TextureCoord);
   vec4 mr = texture(texture_mr, TextureCoord);
   float ao = texture(texture_ao, TextureCoord).r;

   diffuseTex *= ao;
   FragColor = vec4(diffuseTex.rgb, 1.0);
}