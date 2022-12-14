#include "SceneManager.h"
#include <imgui.h>
#include <filesystem>
#include "ModelColorScene.h"
#include "ModelTextureScene.h"
#include "../Camera.h"

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
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	if(pickScenePopup)
	{
		ImGui::OpenPopup("Pick A Scene");

		ImGui::SetNextWindowPos(ImVec2(5, 55));
		if(ImGui::BeginPopupModal("Pick A Scene"))
		{
			ImGui::PushFont(boldFont);
			ImGui::Text("Pick a preferred scene");
			ImGui::PopFont();

			ImGui::Separator();
			ImGui::Text("Loads in a sphere and gives the user material\nproperties like 'Color' to manipulate .");
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Sphere - Simple Color"))
			{
				pickScenePopup = false;
				activeScene = &sphereColorScene;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopFont();

			ImGui::Separator();
			ImGui::Text("Be able to select a model located in 'Resources/Models' to be loaded in.");
			ImGui::Text("Then this model will be loaded without any textures and will be shaded using");
			ImGui::Text("properties that can be changed by the user.");
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Model - Simple Color"))
			{
				pickScenePopup = false;
				modelColorSceneSetup = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopFont();

			ImGui::Separator();
			ImGui::Text("Be able to select a model located in 'Resources/Models' to be loaded in");
			ImGui::Text("together with all textures bound to it in the model file.");
			ImGui::PushFont(boldFont);
			if(ImGui::Button("Model - Load Textures"))
			{
				pickScenePopup = false;
				modelTextureSceneSetup = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopFont();

			ImGui::Separator();
			ImGui::Text("Be able to select a model located in 'Resources/Models' to be loaded in.");
			ImGui::Text("You will also be able to select a texture per texture type.");
			ImGui::Text("Select this if you want to test (PBR) textures that can be used on any model");
			ImGui::PushFont(boldFont);
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
			ImGui::PopFont();

			ImGui::Separator();
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
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];
	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::PushFont(boldFont);
		ImGui::Text("Select a model:");
		ImGui::PopFont();

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
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::PushFont(boldFont);
		ImGui::Text("Select a model:");
		ImGui::PopFont();

		ShowOptionList("Model", modelPaths, selectedModel);
		ImGui::Separator();

		ImGui::Text("The model that gets chosen assumes that the layout for the texture");
		ImGui::Text("types AO, Metallic and roughness are in a ORM layout.");

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
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::OpenPopup("Setup Scene");

	if(ImGui::BeginPopupModal("Setup Scene"))
	{
		ImGui::PushFont(boldFont);
		ImGui::Text("Select a model:");
		ImGui::PopFont();

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
