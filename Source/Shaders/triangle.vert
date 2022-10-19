#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 ModelMatrix;
uniform mat4 VPMatrix;
out vec3 Normal;
out vec2 TextureCoord;

void main()
{
	gl_Position = VPMatrix * ModelMatrix * vec4(aPos, 1.0);
	TextureCoord = aTexCoord;
	Normal = aNormal;
}