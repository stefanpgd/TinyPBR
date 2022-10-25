#pragma once
#include "Scene.h"
#include <glm/glm.hpp>

class SphereColorScene : public Scene
{
public:
	SphereColorScene();

	virtual void Update() override;
	virtual void Draw(Camera* camera) override;

private:
	glm::vec3 sphereColor;
};