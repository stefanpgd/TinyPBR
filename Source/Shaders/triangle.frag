#version 460 core
out vec4 FragColor;
in vec3 Normal;
in vec2 TextureCoord;
uniform sampler2D texture_diffuse;

void main()
{
   	vec4 diffuseTex = texture(texture_diffuse, TextureCoord);
   FragColor = vec4(diffuseTex.rgb, 1.0);
}