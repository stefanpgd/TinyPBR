#pragma once
#include <glm/glm.hpp>
#include <vector>

class ShaderProgram;
struct aiMesh;
struct aiScene;

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextureCoord;
	glm::vec3 Tangent;
};

class AMesh
{
public:
	AMesh(const aiMesh* mesh, const aiScene* scene);

	void Draw(ShaderProgram* shaderProgram);

private:
	void SetupBuffers();

	unsigned int indicesCount = 0;
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indexData;
	unsigned int VBO, VAO, EBO;
};