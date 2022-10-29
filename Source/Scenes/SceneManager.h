#pragma once
#include "SphereColorScene.h"
#include "ModelSelectTextureScene.h"

#include <string>
#include <vector>
#include <array>

class Camera;
class ShaderProgram;

class SceneManager
{
public:
	SceneManager();

	void Update();
	void Draw(Camera* camera);

private:
	void PickScene();
	void GatherAllModelPaths(std::vector<std::string>& files, std::string path, std::string originalPath);
	void GatherAllTexturePaths(std::vector<std::string>& files, std::string path, std::string originalPath);

	// Scene setups //
	void SetupModelColorScene();
	void SetupModelTextureScene();
	void SetupModelSelectScene();

	void ShowOptionList(std::string name, std::vector<std::string> listContent, unsigned int& selectedID);
	
	std::vector<std::string> modelPaths;
	std::vector<std::string> texturePaths;
	std::vector<std::string> modelFileFilters
	{
		".bin", ".jpg", ".jpeg", ".png",
		".md", ".txt"
	};

	std::vector<std::string> rgbChannels
	{
		"R", "G", "B"
	};

	
	Scene* activeScene = nullptr;
	SphereColorScene sphereColorScene;
	ModelSelectTextureData modelSelectData;

	unsigned int selectedModel = 0;
	unsigned int selectedAlbedo = 0;
	unsigned int selectedNormal = 0;
	unsigned int selectedMetallic = 0;
	unsigned int selectedRoughness = 0;
	unsigned int selectedAO = 0;

	bool pickScenePopup = true;
	bool modelColorSceneSetup = false;
	bool modelTextureSceneSetup = false;
	bool modelSelectTextureSceneSetup = false;
};