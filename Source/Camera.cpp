#include "Camera.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float windowWidth, float windowHeight) :
	position(position), front(front), up(up)
{
	viewMatrix = glm::lookAt(position, position + front, up);
	projectionMatrix = glm::perspective(glm::radians(FOV), windowWidth / windowHeight, nearClip, farClip);
}

void Camera::Update(float deltaTime)
{
	viewMatrix = glm::lookAt(position, position + front, up);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void Camera::DebugDrawImGui()
{
	ImGui::Begin("Camera Settings");
	ImGui::DragFloat3("Position", &position[0], 0.01f);
	
	if (ImGui::SliderFloat("FOV", &FOV, 0.0f, 90.0f))
	{
		projectionMatrix = glm::perspective(glm::radians(FOV), windowWidth / windowHeight, nearClip, farClip);
	}

	ImGui::InputFloat("Near Clip", &nearClip);
	ImGui::InputFloat("Far Clip", &farClip);
	ImGui::End();
}

glm::mat4& Camera::GetViewProjectionMatrix()
{
	return viewProjectionMatrix;
}

glm::vec3& Camera::GetPosition()
{
	return position;
}

void Camera::ProcessInput(float deltaTime)
{
}