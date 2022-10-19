#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = delete;
	Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float windowWidth, float windowHeight);

	void Update(float deltaTime);
	void DebugDrawImGui();

	glm::mat4& GetViewProjectionMatrix();
	glm::vec3& GetPosition();

private:
	void ProcessInput(float deltaTime);

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;

	float windowWidth;
	float windowHeight;

	float FOV = 45.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;

	float nearClip = 0.1f;
	float farClip = 100.0f;

	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;
	bool firstMouseMovement = true;
	bool constrainPitch = true;
};