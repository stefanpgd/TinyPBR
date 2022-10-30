#include "ModelColorScene.h"
#include "../Model.h"
#include "../ShaderProgram.h"
#include "../Camera.h"
#include "../Utilities.h"

#include <imgui.h>

ModelColorScene::ModelColorScene(std::string& filePath)
{
	shaderProgram = new ShaderProgram("triangle.vert", "ModelColor.frag");
	models.push_back(new Model(filePath));

	lightPosition = glm::vec3(0.75f, 1.5f, 1.5f);
	lightColor = glm::vec3(1.0f);
}

void ModelColorScene::Update()
{
	ImGui::SetNextWindowPos(ImVec2(0, 340));
	ImGui::SetNextWindowSize(ImVec2(350, 500));

	ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Material Properties");
	ImGui::ColorEdit3("Albedo", &albedo[0]);
	ImGui::SliderFloat("Metalic", &metallic, 0.0f, 1.0f);
	ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
	ImGui::Separator();
	ImGui::Text("Light Settings:");
	ImGui::DragFloat3("Light Position", &lightPosition[0], 0.01f);
	ImGui::ColorEdit3("Light Color", &lightColor[0]);
	ImGui::End();

	models[0]->Update();
}

void ModelColorScene::Draw(Camera* camera)
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	shaderProgram->SetVec3("CameraPosition", camera->GetPosition());

	shaderProgram->SetVec3("albedo", albedo);
	shaderProgram->SetFloat("metallic", metallic);
	shaderProgram->SetFloat("roughness", roughness);

	shaderProgram->SetVec3("lightPosition", lightPosition);
	shaderProgram->SetVec3("lightColor", lightColor);

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(shaderProgram);
	}
}