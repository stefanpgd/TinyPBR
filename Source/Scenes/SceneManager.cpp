#include "SceneManager.h"
#include <imgui.h>

void SceneManager::Update()
{
	if(activeScene)
	{
		activeScene->Update();
	}
	else
	{
		PickScene();
	}

	ImGui::BeginMainMenuBar();

	if(ImGui::Button("New Scene"))
	{
		activeScene = nullptr;
	}

	ImGui::EndMainMenuBar();
}

void SceneManager::Draw(Camera* camera)
{
	if(activeScene)
	{
		activeScene->Draw(camera);
	}
}

void SceneManager::PickScene()
{
	ImGui::OpenPopup("Pick A Scene");

	bool isOpen = true;
	if(ImGui::BeginPopupModal("Pick A Scene", &isOpen, ImGuiWindowFlags_Modal))
	{
		ImGui::Text("Pick a preferred scene");

		if(ImGui::Button("Sphere Color"))
		{
			activeScene = &sphereColorScene;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}