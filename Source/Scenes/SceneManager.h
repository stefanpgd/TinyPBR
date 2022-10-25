#pragma once
#include "SphereColorScene.h"
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
	
	std::vector<std::string> modelPaths;
	std::array<std::string, 4> modelFileFilters
	{
		".bin", ".jpg", ".jpeg", ".png"
	};

	std::vector<std::string> texturePaths;

	Scene* activeScene = nullptr;
	SphereColorScene sphereColorScene;

	unsigned int selectedModel = 0;

	bool pickScenePopup = true;
	bool modelColorSceneSetup = false;
};