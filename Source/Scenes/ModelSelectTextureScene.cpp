#include "ModelSelectTextureScene.h"
#include "../Model.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Utilities.h"
#include "../Texture.h"
#include <imgui.h>

ModelSelectTextureScene::ModelSelectTextureScene(ModelSelectTextureData data)
{
	modelData = data;
	shaderProgram = new ShaderProgram("triangle.vert", "ModelTextureSelect.frag");
	models.push_back(new Model(data.modelPath, false));

	for(int i = 0; i < 3; i++)
	{
		lightColors[i] = glm::vec3(1.0f);
		lightIntensities[i] = 1.0f;
	}

	lightPositions[0] = glm::vec3(0.0f, 2.0f, 0.0f);
	lightPositions[1] = glm::vec3(0.5f, 1.0f, 1.5f);
	lightPositions[2] = glm::vec3(-0.5f, -1.0f, 1.5f);

	textures.push_back(new Texture(data.albedoPath, TextureType::Albedo, true));
	textures.push_back(new Texture(data.normalPath, TextureType::Normal, false));
	textures.push_back(new Texture(data.metallicPath, TextureType::Metallic, false));
	textures.push_back(new Texture(data.roughnessPath, TextureType::Roughness, false));
	textures.push_back(new Texture(data.ambientOcclusionPath, TextureType::AmbientOcclusion, false));
}

void ModelSelectTextureScene::Update()
{
	ImGui::Begin("Scene Settings");
	ImGui::Text("Light Settings:");

	ImGui::ShowDemoWindow();

	for(int i = 0; i < 3; i++)
	{
		std::string text = "Light - " + std::to_string(i);

		if(ImGui::TreeNode(text.c_str()))
		{
			ImGui::DragFloat3("Light Position", &lightPositions[i][0], 0.01f);
			ImGui::ColorEdit3("Light Color", &lightColors[i][0]);
			ImGui::DragFloat("Light Intensity", &lightIntensities[i], 0.1f, 0.0f);
			ImGui::TreePop();
		}
	}

	ImGui::Separator();
	ImGui::DragFloat2("Texture Coord Multiplier", &texCoordMultiplier[0], 0.01f, 0.01f, 10.0f);
	ImGui::End();

	models[0]->Update();
}

void ModelSelectTextureScene::Draw(Camera* camera)
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	shaderProgram->SetVec3("CameraPosition", camera->GetPosition());
	shaderProgram->SetInt("metallicChannel", modelData.metallicChannel);
	shaderProgram->SetInt("roughnessChannel", modelData.roughnessChannel);
	shaderProgram->SetInt("aoChannel", modelData.aoChannel);
	shaderProgram->SetVec2("texMultiplier", texCoordMultiplier);

	for(int i = 0; i < 3; i++)
	{
		shaderProgram->SetVec3("lightPosition[" + std::to_string(i) + "]", lightPositions[i]);
		shaderProgram->SetVec3("lightColor[" + std::to_string(i) + "]", lightColors[i]);
		shaderProgram->SetFloat("lightIntensity[" + std::to_string(i) + "]", lightIntensities[i]);
	}

	for(int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(shaderProgram);
	}

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(shaderProgram);
	}
}