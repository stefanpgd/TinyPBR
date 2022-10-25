#pragma once
#include <vector>

class ShaderProgram;
class Model;
class Camera;

class Scene
{
public:
	virtual void Update() = 0;
	virtual void Draw(Camera* camera) = 0;

protected:
	std::vector<Model*> models;
	ShaderProgram* shaderProgram;
};