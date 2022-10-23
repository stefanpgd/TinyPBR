#include "Model.h"

#include <cassert>
#include <tiny_gltf.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "ShaderProgram.h"

Model::Model(std::string file)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF modelLoader;

	std::string warning;
	std::string error;

	bool loadedModel = modelLoader.LoadASCIIFromFile(&model, &error, &warning, file);

	if (!warning.empty() || !error.empty())
	{
		printf("TinyglTF Warning: %s.\n", warning.c_str());
		printf("TinyglTF Error: %s!\n", error.c_str());
	}

	if (!loadedModel)
	{
		assert(false && "Failed to load glTF model");
	}

	for (auto& mesh : model.meshes)
	{
		std::vector<Mesh*> prims;
		for (auto& primitive : mesh.primitives)
		{
			prims.push_back(new Mesh(&model, primitive, file));
		}
		meshes.push_back(prims);
	}

	for (int i = 0; i < model.scenes[model.defaultScene].nodes.size(); i++)
	{
		sceneRootNodes.push_back(model.scenes[model.defaultScene].nodes[i]);
	}

	// Retrieve the (local-)transform data per Node // 
	nodes.resize(model.nodes.size());
	for (int i = 0; i < model.nodes.size(); i++)
	{
		for (int j = 0; j < model.nodes[i].children.size(); j++)
		{
			nodes[i].Children.push_back(model.nodes[i].children[j]);
		}

		nodes[i].MeshID = model.nodes[i].mesh;

		auto transform = model.nodes[i];
		glm::vec3 position(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation(0.0f, 0.0f, 0.0f);
		glm::vec3 scale(1.0f, 1.0f, 1.0f);

		if (transform.translation.size() > 0)
		{
			position = glm::vec3(transform.translation[0], transform.translation[1], transform.translation[2]);
		}

		if (transform.rotation.size() > 0)
		{
			rotation = glm::vec3(transform.rotation[0], transform.rotation[1], transform.rotation[2]);
		}

		if (transform.scale.size() > 0)
		{
			scale = glm::vec3(transform.scale[0], transform.scale[1], transform.scale[2]);
		}

		auto& model = nodes[i].LocalTransform;
		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
	}
}

void Model::Draw(const ShaderProgram* shaderProgram)
{
	transform.DebugDrawImGui();

	for (int i = 0; i < sceneRootNodes.size(); i++)
	{
		DrawSceneNode(shaderProgram, sceneRootNodes[i], transform.GetModelMatrix());
	}
}

void Model::DrawSceneNode(const ShaderProgram* shaderProgram, int nodeIndex, glm::mat4& parentTransform)
{
	auto& node = nodes[nodeIndex];

	if (node.MeshID > -1)
	{
		glm::mat4 model = parentTransform * node.LocalTransform;
		shaderProgram->SetMat4("ModelMatrix", model);

		for (auto& mesh : meshes[node.MeshID])
		{
			mesh->Draw(shaderProgram);
		}

		for (int i = 0; i < node.Children.size(); i++)
		{
			DrawSceneNode(shaderProgram, node.Children[i], model);
		}
	}
}