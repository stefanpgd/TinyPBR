#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
class Mesh;
class Camera;
class ShaderProgram;

struct MeshNode
{
	glm::mat4 localTransform;
	std::vector<MeshNode> children;
	int meshIndex;
};

class Model
{
public:
	Model() = delete;
	Model(const std::string& filePath);

	void Draw(const Camera* camera, const ShaderProgram* shaderProgram);
	void DebugDrawImGui();

private:
	std::vector<Mesh*> meshes;
	std::vector<MeshNode> nodes;

	glm::mat4 model; // replace with transform component;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale{1.0f, 1.0f, 1.0f};
};