#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"

class ShaderProgram;
class Texture;

struct aiMesh;
struct aiScene;
struct aiMaterial;

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
	Mesh(aiMesh* mesh, const aiScene* scene, std::string& modelFilePath, bool loadTextures = false);

	void Draw(ShaderProgram* shaderProgram);

private:
	void ProcessMaterials(aiMesh* mesh, const aiScene* scene);
	void SetupBuffers();
	void LoadTexture(aiMaterial* material, TextureType type);

	unsigned int indicesCount = 0;
	unsigned int VBO, VAO, EBO;

	std::string modelFilePath;
	std::vector<VertexData> vertexData;
	std::vector<unsigned int> indexData;
	std::vector<Texture*> textures;
};