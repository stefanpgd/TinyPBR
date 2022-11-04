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
	channelViewProgram = new ShaderProgram("triangle.vert", "TextureViewMode.frag");
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
	ImGui::SetNextWindowPos(ImVec2(0, 340));
	ImGui::SetNextWindowSize(ImVec2(350, 500));

	ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Light Settings:");

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
	// Check if we want to view a specific channel, if so, bind the channelViewProgram
	if(ChannelViewMode())
	{
		activeShader = channelViewProgram;
		activeShader->Bind();
		activeShader->SetBool("texturesBoundToModel", false);
		activeShader->SetInt("viewMode", currentChannel);
	}
	else
	{
		activeShader = shaderProgram;
		activeShader->Bind();
	}

	activeShader->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	activeShader->SetVec3("CameraPosition", camera->GetPosition());
	activeShader->SetInt("metallicChannel", modelData.metallicChannel);
	activeShader->SetInt("roughnessChannel", modelData.roughnessChannel);
	activeShader->SetInt("aoChannel", modelData.aoChannel);
	activeShader->SetVec2("texMultiplier", texCoordMultiplier);

	for(int i = 0; i < 3; i++)
	{
		activeShader->SetVec3("lightPosition[" + std::to_string(i) + "]", lightPositions[i]);
		activeShader->SetVec3("lightColor[" + std::to_string(i) + "]", lightColors[i]);
		activeShader->SetFloat("lightIntensity[" + std::to_string(i) + "]", lightIntensities[i]);
	}

	for(int i = 0; i < textures.size(); i++)
	{
		textures[i]->Bind(activeShader);
	}

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(activeShader);
	}
}