#include "ModelTextureScene.h"
#include "../Model.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Utilities.h"

#include <imgui.h>

ModelTextureScene::ModelTextureScene(std::string& filePath)
{
	shaderProgram = new ShaderProgram("triangle.vert", "ModelTexture.frag");
	models.push_back(new Model(filePath, true));
	
	for(int i = 0; i < 3; i++)
	{
		lightColors[i] = glm::vec3(1.0f);
		lightIntensities[i] = 1.0f;
	}

	lightPositions[0] = glm::vec3(0.0f, 2.0f, 0.0f);
	lightPositions[1] = glm::vec3(0.5f, 1.0f, 1.5f);
	lightPositions[2] = glm::vec3(-0.5f, 1.0f, 1.5f);
}

void ModelTextureScene::Update()
{
	ImGui::SetNextWindowPos(ImVec2(0, 340));
	ImGui::SetNextWindowSize(ImVec2(350, 500));

	ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Light Settings:");

	ImGui::ShowDemoWindow();

	for(int i = 0; i < 3; i++)
	{
		std::string text = "Light - " + std::to_string(i);

		if(ImGui::TreeNode(text.c_str()))
		{
			ImGui::DragFloat3("Light Position", &lightPositions[i][0], 0.01f);
			ImGui::ColorEdit3("Light Color", &lightColors[i][0]);
			ImGui::DragFloat("Light Intensity", &lightIntensities[i], 0.1f);
			ImGui::TreePop();
		}
	}

	ImGui::End();

	models[0]->Update();
}

void ModelTextureScene::Draw(Camera* camera)
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	shaderProgram->SetVec3("CameraPosition", camera->GetPosition());

	for(int i = 0; i < 3; i++)
	{
		shaderProgram->SetVec3("lightPosition[" + std::to_string(i) + "]", lightPositions[i]);
		shaderProgram->SetVec3("lightColor[" + std::to_string(i) + "]", lightColors[i]);
		shaderProgram->SetFloat("lightIntensity[" + std::to_string(i) + "]", lightIntensities[i]);
	}

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(shaderProgram);
	}
}