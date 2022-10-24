#include "Mesh.h"

#include <glad/glad.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Mesh::Mesh(aiMesh* mesh)
{
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

	SetupBuffers();
	indicesCount = indexData.size();
	vertexData.clear();
	indexData.clear();
}

void Mesh::Draw(ShaderProgram* shaderProgram)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
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