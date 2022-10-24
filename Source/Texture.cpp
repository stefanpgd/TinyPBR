#include "Texture.h"
#include <cassert>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include <stb_image.h>

Texture::Texture(std::string& filePath, TextureType type)
{
	this->type = type;

	buffer = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	
	if (buffer == nullptr)
	{
		assert(false && "Failed to load texture.");
	}

	// Generate a texture ID to use
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int format = channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(buffer);
}

void Texture::Bind(const ShaderProgram* program)
{
	unsigned int typeValue = (unsigned int)type;
	glActiveTexture(GL_TEXTURE0 + typeValue);
	glBindTexture(GL_TEXTURE_2D, ID);
	program->SetInt(bindNames[int(typeValue)], typeValue);
}