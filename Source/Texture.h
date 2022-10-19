#pragma once
#include <string>

class ShaderProgram;

enum class TextureType
{
	Albedo = 0,
	Normal,
	MetalicRoughness,
	AmbientOcclusion
};

class Texture
{
public:
	Texture(std::string& filePath, TextureType type);
	void Bind(const ShaderProgram* program);

private:
	TextureType type;
	unsigned int ID;
	int width;
	int height;
	int channels;
	unsigned char* buffer;

	std::string bindNames[4]
	{
		"texture_albedo",
		"texture_normal",
		"texture_mr",
		"texture_ao"
	};
};