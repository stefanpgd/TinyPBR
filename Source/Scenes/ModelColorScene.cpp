#include "ModelColorScene.h"
#include "../Model.h"
#include "../ShaderProgram.h"
#include "../Camera.h"

#include <imgui.h>

ModelColorScene::ModelColorScene(std::string& filePath)
{
	shaderProgram = new ShaderProgram("triangle.vert", "SphereColor.frag");
	models.push_back(new Model(filePath));
}

void ModelColorScene::Update()
{
	ImGui::Begin("Scene Settings");
	ImGui::ColorPicker3("Sphere Color", &modelColor[0]);
	ImGui::End();
}

void ModelColorScene::Draw(Camera* camera)
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	shaderProgram->SetVec3("Color", modelColor);

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(shaderProgram);
	}
}