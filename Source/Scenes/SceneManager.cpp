#include "SceneManager.h"
#include <imgui.h>
#include <filesystem>
#include "ModelColorScene.h"
#include "ModelTextureScene.h"

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

	if(ImGui::Button("Refresh Model & Texture paths"))
	{
		GatherAllModelPaths(modelPaths, "Resources/Models", "Resources/Models");
		GatherAllTexturePaths(texturePaths, "Resources/Textures", "Resources/Textures");
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

			if(ImGui::Button("Sphere - Select Color"))
			{
				pickScenePopup = false;
				activeScene = &sphereColorScene;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Button("Model - Select Color"))
			{
				pickScenePopup = false;
				modelColorSceneSetup = true;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Button("Model - Load Textures"))
			{
				pickScenePopup = false;
				modelTextureSceneSetup = true;
				ImGui::CloseCurrentPopup();
			}

			if(ImGui::Button("Model - Select Textures"))
			{
				pickScenePopup = false;
				modelSelectTextureSceneSetup = true;
				selectedAlbedo = texturePaths.size() - 1;
				selectedNormal = texturePaths.size() - 1;
				selectedMetallic = texturePaths.size() - 1;
				selectedRoughness = texturePaths.size() - 1;
				selectedAO = texturePaths.size() - 1;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	if(modelColorSceneSetup)
	{
		SetupModelColorScene();
	}

	if(modelTextureSceneSetup)
	{
		SetupModelTextureScene();
	}

	if(modelSelectTextureSceneSetup)
	{
		SetupModelSelectScene();
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
	for(const auto& file : std::filesystem::directory_iterator(path))
	{
		if(file.is_directory())
		{
			GatherAllTexturePaths(files, file.path().string(), originalPath);
			continue;
		}

		std::string filePath = file.path().string();
		filePath.erase(0, originalPath.size());
		std::string fileType = strrchr(filePath.c_str(), '.');

		texturePaths.push_back(filePath);
	}
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

void SceneManager::SetupModelTextureScene()
{
	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::Text("Pick a preferred model");

		ShowOptionList("Model", modelPaths, selectedModel);

		if(ImGui::Button("Load Scene"))
		{
			std::string filePath = "Resources/Models" + modelPaths[selectedModel];
			activeScene = new ModelTextureScene(filePath);
			modelTextureSceneSetup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void SceneManager::SetupModelSelectScene()
{
	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::Text("Select a model:");
		ShowOptionList("Model", modelPaths, selectedModel);

		ImGui::Separator();

		if(texturePaths.size() > 0)
		{
			ImGui::Text("Select the textures you want to use.");
			ImGui::Text("If you're missing a type, just leave it to 'None'");

			ShowOptionList("Albedo", texturePaths, selectedAlbedo);
			ShowOptionList("Normal", texturePaths, selectedNormal);
			ShowOptionList("Metallic", texturePaths, selectedMetallic);
			ShowOptionList("Roughness", texturePaths, selectedRoughness);
			ShowOptionList("Ambient Occlusion", texturePaths, selectedAO);

			ImGui::Separator();
			ImGui::Text("Since some textures might be multiple types in one ( ORM, MR enc. )");
			ImGui::Text("Select the channels that will be used:");
			ShowOptionList("Metallic Channel", rgbChannels, modelSelectData.metallicChannel);
			ShowOptionList("Roughness Channel", rgbChannels, modelSelectData.roughnessChannel);
			ShowOptionList("Ambient Occlusion Channel", rgbChannels, modelSelectData.aoChannel);

			if(ImGui::Button("Load Scene"))
			{
				std::string filePath = "Resources/Models" + modelPaths[selectedModel];
				std::string texturePrefix = "Resources/Textures";

				modelSelectData.modelPath = filePath;
				modelSelectData.albedoPath = texturePrefix + texturePaths[selectedAlbedo];
				modelSelectData.normalPath = texturePrefix + texturePaths[selectedNormal];
				modelSelectData.metallicPath = texturePrefix + texturePaths[selectedMetallic];
				modelSelectData.roughnessPath = texturePrefix + texturePaths[selectedRoughness];
				modelSelectData.ambientOcclusionPath = texturePrefix + texturePaths[selectedAO];

				activeScene = new ModelSelectTextureScene(modelSelectData);
				modelSelectTextureSceneSetup = false;
				ImGui::CloseCurrentPopup();
			}
		}
		else
		{
			ImGui::Text("You've no textures in the 'Resources/Textures' folder");
			ImGui::Text("So the scene can't be loaded.");
		}

		ImGui::EndPopup();
	}
}

void SceneManager::ShowOptionList(std::string name, std::vector<std::string> listContent, unsigned int& selectedID)
{
	if(listContent.size() > 0)
	{
		if(ImGui::BeginCombo(name.c_str(), listContent[selectedID].c_str()))
		{
			for(int i = 0; i < listContent.size(); i++)
			{
				bool isSelected = (listContent[selectedID] == listContent[i]);
				if(ImGui::Selectable(listContent[i].c_str(), isSelected))
				{
					selectedID = i;
				}

				if(isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}
	}
}
