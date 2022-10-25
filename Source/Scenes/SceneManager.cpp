#include "SceneManager.h"
#include <imgui.h>
#include <filesystem>
#include "ModelColorScene.h"

SceneManager::SceneManager()
{
	GatherAllModelPaths(modelPaths, "Resources/Models", "Resources/Models");
	GatherAllTexturePaths(texturePaths, "Resources/Textures", "Resources/Textures");
}

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
		pickScenePopup = true;
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
	if(pickScenePopup)
	{
		ImGui::OpenPopup("Pick A Scene");
		if(ImGui::BeginPopupModal("Pick A Scene"))
		{
			ImGui::Text("Pick a preferred scene");

			if(ImGui::Button("Sphere Color"))
			{
				pickScenePopup = false;
				activeScene = &sphereColorScene;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Button("Model Color"))
			{
				pickScenePopup = false;
				modelColorSceneSetup = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	
	if(modelColorSceneSetup)
	{
		SetupModelColorScene();
	}
}

void SceneManager::GatherAllModelPaths(std::vector<std::string>& files, std::string path, std::string originalPath)
{
	for(const auto& file : std::filesystem::directory_iterator(path))
	{
		if(file.is_directory())
		{
			GatherAllModelPaths(files, file.path().string(), originalPath);
			continue;
		}

		std::string filePath = file.path().string();
		filePath.erase(0, originalPath.size());
		std::string fileType = strrchr(filePath.c_str(), '.');

		bool addToList = true;
		for(int i = 0; i < modelFileFilters.size(); i++)
		{
			if(fileType == modelFileFilters[i])
			{
				addToList = false;
			}
		}

		if(addToList)
		{
			modelPaths.push_back(filePath);
		}
	}
}

void SceneManager::GatherAllTexturePaths(std::vector<std::string>& files, std::string path, std::string originalPath)
{
}

void SceneManager::SetupModelColorScene()
{
	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::Text("Pick a preferred model");

		if(ImGui::BeginCombo("Models", modelPaths[selectedModel].c_str()))
		{
			for(int i = 0; i < modelPaths.size(); i++)
			{
				bool isSelected = (modelPaths[selectedModel] == modelPaths[i]);
				if(ImGui::Selectable(modelPaths[i].c_str(), isSelected))
				{
					selectedModel = i;
				}

				if(isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		if(ImGui::Button("Load Scene"))
		{
			std::string filePath = "Resources/Models" + modelPaths[selectedModel];
			activeScene = new ModelColorScene(filePath);
			modelColorSceneSetup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}