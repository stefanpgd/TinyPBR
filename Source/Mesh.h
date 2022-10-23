#pragma once
#include <glm/glm.hpp>
#include <tiny_gltf.h>
#include <vector>
#include <string>

class Texture;
class ShaderProgram;
enum class TextureType;

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
	Mesh(tinygltf::Model* model, tinygltf::Primitive primitive, std::string modelPath, bool hasTextures = true);
	void Draw(const ShaderProgram* shaderProgram);

private:
	void GetVertices(tinygltf::Model* model, tinygltf::Primitive& primitive);
	void GetIndices(tinygltf::Accessor& accessor, tinygltf::Buffer& buffer, tinygltf::BufferView& bufferView);
	void ParseModelData(tinygltf::Accessor& accessor, tinygltf::BufferView& bufferView, tinygltf::Buffer& buffer, const std::string& type);

	void SetupMesh();
	void LoadTexture(tinygltf::Model* model, std::string modelPath, TextureType type, int textureID);

	glm::mat4 local;
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indexData;
	std::vector<Texture*> textures;

	unsigned int VBO, VAO, EBO;
	unsigned int indicesCount;
};