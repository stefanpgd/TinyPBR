#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 ModelMatrix;
uniform mat4 VPMatrix;
out vec3 vertexColor;

void main()
{
	gl_Position = VPMatrix * ModelMatrix * vec4(aPos, 1.0);
	vertexColor = aColor;
}