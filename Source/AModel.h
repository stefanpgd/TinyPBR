#pragma once
#include <vector>
#include <string>

class ShaderProgram;
class AMesh;
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class AModel
{
public:
	AModel(const std::string& filePath);
	
	void Draw(ShaderProgram* shaderProgram);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);

	std::vector<AMesh*> meshes;
};