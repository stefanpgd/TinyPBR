#pragma once
#include <glm/glm.hpp>
#include <tiny_gltf.h>
#include <vector>
#include <string>

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoord;
	glm::vec3 Tangent;
};

class Mesh
{
public:
	Mesh(tinygltf::Model* model, tinygltf::Primitive primitive, std::string modelPath);

	void Draw();

private:
	void GetVertices(tinygltf::Model* model, tinygltf::Primitive& primitive);
	void GetIndices(tinygltf::Accessor& accessor, tinygltf::Buffer& buffer, tinygltf::BufferView& bufferView);
	void ParseModelData(tinygltf::Accessor& accessor, tinygltf::BufferView& bufferView, tinygltf::Buffer& buffer, const std::string& type);

	void SetupMesh();

	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indexData;

	unsigned int VBO, VAO, EBO;
	unsigned int indicesCount;
};