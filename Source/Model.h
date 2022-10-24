#pragma once
#include <vector>
#include <string>
#include "Transform.h"

class ShaderProgram;
class Mesh;

// Assimp // 
struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

class Model
{
public:
	Model(const std::string& filePath);
	
	void Draw(ShaderProgram* shaderProgram);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);

	Transform transform;
	std::vector<Mesh*> meshes;
};