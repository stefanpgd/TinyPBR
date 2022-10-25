#pragma once
#include "SphereColorScene.h"

class Camera;
class ShaderProgram;

class SceneManager
{
public:
	void Update();
	void Draw(Camera* camera);

private:
	void PickScene();

	Scene* activeScene = nullptr;
	SphereColorScene sphereColorScene;
};