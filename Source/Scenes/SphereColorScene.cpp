#include "SphereColorScene.h"
#include "../Model.h"
#include "../ShaderProgram.h"
#include "../Camera.h"

#include <imgui.h>

SphereColorScene::SphereColorScene()
{
	shaderProgram = new ShaderProgram("triangle.vert", "SphereColor.frag");
	models.push_back(new Model("Resources/Models/Sphere/sphere.gltf"));
}

void SphereColorScene::Update()
{
	ImGui::Begin("Scene Settings");
	ImGui::ColorPicker3("Sphere Color", &sphereColor[0]);
	ImGui::End();
}

void SphereColorScene::Draw(Camera* camera)
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	shaderProgram->SetVec3("Color", sphereColor);

	for(int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(shaderProgram);
	}
}