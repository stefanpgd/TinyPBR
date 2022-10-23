#include "Model.h"
#include <tiny_gltf.h>
#include <iostream>
#include <cassert>
#include <imgui.h>
#include "Mesh.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>

Model::Model(const std::string& filePath)
{
	tinygltf::Model* model = new tinygltf::Model();
	tinygltf::TinyGLTF modelLoader;

	std::string warning;
	std::string error;

	if (modelLoader.LoadASCIIFromFile(model, &error, &warning, filePath))
	{
		for (int i = 0; i < model->meshes.size(); i++)
		{
			for (int j = 0; j < model->meshes[i].primitives.size(); j++)
			{
				meshes.push_back(new Mesh(model, model->meshes[i].primitives[j], filePath, i));
			}
		}
	}
	else
	{
		printf(warning.c_str());
		printf(error.c_str());
		assert(false && "Failed to load model!");
		return;
	}
}

void Model::Draw(const Camera* camera, const ShaderProgram* shaderProgram)
{
	//rotation.x += 0.5f;
	//rotation.y += 0.2f;
	//rotation.z += 0.15f;

	glm::mat4 model(1.0f);
	model = glm::translate(model, position);

	// Temp
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(shaderProgram, model);
	}
}

void Model::DebugDrawImGui()
{
	ImGui::Begin("Model Settings");
	ImGui::DragFloat3("Position", &position[0], 0.01f);
	ImGui::DragFloat3("Rotation", &rotation[0], 1.0f);
	ImGui::DragFloat3("Scale", &scale[0], 0.01f);
	ImGui::End();
}