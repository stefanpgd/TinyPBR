#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Transform.h"

class Mesh;
class ShaderProgram;

struct MeshNode
{
	int MeshID = -1;
	glm::mat4 LocalTransform;
	std::vector<unsigned int> Children;
};

class Model
{
public:
	Model() = delete;
	Model(std::string file);

	void Draw(const ShaderProgram* shaderProgram);

private:
	void DrawSceneNode(const ShaderProgram* shaderProgram, int nodeIndex, glm::mat4& parentTransform);

	Transform transform;

	std::vector<std::vector<Mesh*>> meshes;
	std::vector<MeshNode> nodes;
	std::vector<unsigned int> sceneRootNodes;
};