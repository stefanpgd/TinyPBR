#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Mesh::Mesh(aiMesh* mesh, const aiScene* scene, std::string& modelFilePath, bool loadTextures)
{
	this->modelFilePath = modelFilePath;

	vertexData.resize(mesh->mNumVertices);

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertexData[i].Position.x = mesh->mVertices[i].x;
		vertexData[i].Position.y = mesh->mVertices[i].y;
		vertexData[i].Position.z = mesh->mVertices[i].z;

		vertexData[i].Normal.x = mesh->mNormals[i].x;
		vertexData[i].Normal.y = mesh->mNormals[i].y;
		vertexData[i].Normal.z = mesh->mNormals[i].z;

		vertexData[i].TextureCoord.x = mesh->mTextureCoords[0][i].x;
		vertexData[i].TextureCoord.y = mesh->mTextureCoords[0][i].y;

		if(mesh->mTangents != nullptr)
		{
			vertexData[i].Tangent.x = mesh->mTangents[i].x;
			vertexData[i].Tangent.y = mesh->mTangents[i].y;
			vertexData[i].Tangent.z = mesh->mTangents[i].z;
		}
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indexData.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	if(loadTextures)
	{
		ProcessMaterials(mesh, scene);
	}

	SetupBuffers();
	indicesCount = indexData.size();

	vertexData.clear();
	vertexData.shrink_to_fit();

	indexData.clear();
	indexData.shrink_to_fit();
}

void Mesh::Draw(ShaderProgram* shaderProgram)
{
	for(int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(shaderProgram);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
}

void Mesh::ProcessMaterials(aiMesh* mesh, const aiScene* scene)
{
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadTexture(material, TextureType::Albedo);
		LoadTexture(material, TextureType::MetalicRoughness);
		LoadTexture(material, TextureType::Normal);
		LoadTexture(material, TextureType::AmbientOcclusion);
	}
	else
	{
		printf("Warning: You're trying to load in textures from a model that doesn't have any materials/textures bound to it.\n");
	}
}

void Mesh::SetupBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexData), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), &indexData[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);

	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));

	// Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 6));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 8));

	glBindVertexArray(0);
}

void Mesh::LoadTexture(aiMaterial* material, TextureType type)
{
	aiTextureType textureType;
	bool loadSRGB = false;

	switch(type)
	{
	case TextureType::Albedo:
		textureType = aiTextureType_DIFFUSE;
		loadSRGB = true;
		break;

	case TextureType::MetalicRoughness:
		textureType = aiTextureType_DIFFUSE_ROUGHNESS;
		break;

	case TextureType::Normal:
		textureType = aiTextureType_NORMALS;
		break;

	case TextureType::AmbientOcclusion:
		textureType = aiTextureType_AMBIENT_OCCLUSION;
		break;

	case TextureType::Emissive:
		textureType = aiTextureType_EMISSIVE;
		break;
	}

	if(material->GetTextureCount(textureType) > 0)
	{
		aiString str;
		material->GetTexture(textureType, 0, &str);

		std::string folder = modelFilePath.substr(0, modelFilePath.find_last_of("\\"));
		std::string path = folder + "/" + str.C_Str();

		Texture* texture = new Texture(path, type, loadSRGB);
		textures.push_back(texture);
	}
	else
	{
		printf("This model doesn't have this texture type bound to it\n");
	}
}