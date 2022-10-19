#include "Mesh.h"
#include <cassert>
#include <glad/glad.h>
#include "Texture.h"
#include "ShaderProgram.h"

static std::vector<std::string> g_MeshAttributes
{
	"POSITION",
	"NORMAL",
	"TEXCOORD_0",
	"TANGENT",
};

Mesh::Mesh(tinygltf::Model* model, tinygltf::Primitive primitive, std::string modelPath)
{
	tinygltf::Accessor& accessor = model->accessors[primitive.indices];
	tinygltf::BufferView& bufferView = model->bufferViews[accessor.bufferView];
	tinygltf::Buffer& buffer = model->buffers[bufferView.buffer];

	vertexData.resize(model->accessors[primitive.attributes.at(g_MeshAttributes[0])].count);
	GetVertices(model, primitive);
	GetIndices(accessor, buffer, bufferView);

	// Calculate Tangents? 

	SetupMesh();

	indicesCount = indexData.size();

	// Clean up vertex/index memory // 
	vertexData.clear();
	indexData.clear();

	int albedoID = model->materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
	int normalID = model->materials[primitive.material].normalTexture.index;
	int metallicRoughnessID = model->materials[primitive.material].pbrMetallicRoughness.metallicRoughnessTexture.index;
	int ambientOcculusionID = model->materials[primitive.material].occlusionTexture.index;
	int emissiveID = model->materials[primitive.material].emissiveTexture.index;

	LoadTexture(model, modelPath, TextureType::Albedo, albedoID);
	LoadTexture(model, modelPath, TextureType::Normal, normalID);
	LoadTexture(model, modelPath, TextureType::MetalicRoughness, metallicRoughnessID);
	LoadTexture(model, modelPath, TextureType::AmbientOcclusion, ambientOcculusionID);
	LoadTexture(model, modelPath, TextureType::Emissive, emissiveID);
}

void Mesh::Draw(const ShaderProgram* shaderProgram)
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(shaderProgram);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
}

void Mesh::GetVertices(tinygltf::Model* model, tinygltf::Primitive& primitive)
{
	for (int i = 0; i < g_MeshAttributes.size(); i++)
	{
		auto attribute = primitive.attributes.find(g_MeshAttributes[i]);

		if (attribute == primitive.attributes.end())
			continue;

		tinygltf::Accessor& accessor = model->accessors[primitive.attributes.at(g_MeshAttributes[i])];
		tinygltf::BufferView& bufferView = model->bufferViews[accessor.bufferView];

		if (accessor.count != vertexData.size())
		{
			assert(false && "Attribute doesn't match with the amount of vertices in the model");
		}

		ParseModelData(accessor, bufferView, model->buffers[bufferView.buffer], g_MeshAttributes[i]);
	}
}

void Mesh::GetIndices(tinygltf::Accessor& accessor, tinygltf::Buffer& buffer, tinygltf::BufferView& bufferView)
{
	size_t start = bufferView.byteOffset;
	size_t end = bufferView.byteOffset + bufferView.byteLength;
	unsigned int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType);

	unsigned int tempInt;
	unsigned short tempShort;
	for (size_t i = bufferView.byteOffset; i < bufferView.byteOffset + bufferView.byteLength; i += componentSize)
	{
		if (componentSize == 2)
		{
			memcpy(&tempShort, &buffer.data[i], componentSize);
			indexData.push_back(tempShort);
		}
		else if (componentSize == 4)
		{
			memcpy(&tempInt, &buffer.data[i], componentSize);
			indexData.push_back(tempInt);
		}
	}
}

void Mesh::ParseModelData(tinygltf::Accessor& accessor, tinygltf::BufferView& bufferView, tinygltf::Buffer& buffer, const std::string& type)
{
	int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType);
	int Stride = accessor.ByteStride(bufferView);
	size_t offset = bufferView.byteOffset + accessor.byteOffset;

	for (int i = 0; i < vertexData.size(); i++)
	{
		size_t location = bufferView.byteOffset + accessor.byteOffset + i * Stride;

		if (type == "POSITION")
		{
			vertexData[i].Position.x = *(float*)(&buffer.data[location]);
			vertexData[i].Position.y = *(float*)(&buffer.data[location + componentSize]);
			vertexData[i].Position.z = *(float*)(&buffer.data[location + componentSize * 2]);
		}
		else if (type == "NORMAL")
		{
			vertexData[i].Normal.x = *(float*)(&buffer.data[location]);
			vertexData[i].Normal.y = *(float*)(&buffer.data[location + componentSize]);
			vertexData[i].Normal.z = *(float*)(&buffer.data[location + componentSize * 2]);
		}
		else if (type == "TEXCOORD_0")
		{
			vertexData[i].TextureCoord.x = *(float*)(&buffer.data[location]);
			vertexData[i].TextureCoord.y = *(float*)(&buffer.data[location + componentSize]);
		}
		else if (type == "TANGENT")
		{
			vertexData[i].Tangent.x = *(float*)(&buffer.data[location]);
			vertexData[i].Tangent.y = *(float*)(&buffer.data[location + componentSize]);
			vertexData[i].Tangent.z = *(float*)(&buffer.data[location + componentSize * 2]);
		}
	}
}

void Mesh::SetupMesh()
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

void Mesh::LoadTexture(tinygltf::Model* model, std::string modelPath, TextureType type, int textureID)
{
	Texture* texture;
	if (textureID != -1)
	{
		int image = model->textures[textureID].source;
		std::string textureFile = model->images[image].uri;
		std::string dir = modelPath.substr(0, modelPath.find_last_of("/") + 1);
		std::string path = dir + textureFile;

		texture = new Texture(path, type);
		textures.push_back(texture);
	}
	else
	{
		printf("Warning: Model doesn't contain this type of texture!");
	}
}