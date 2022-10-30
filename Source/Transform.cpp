#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_internal.h>

Transform::Transform()
{
	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);

	model = glm::mat4(1.0f);
}

void Transform::DebugDrawImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::SetNextWindowPos(ImVec2(0, 40));
	ImGui::SetNextWindowSize(ImVec2(350, 130));
	ImGui::Begin("Object Transform", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushFont(boldFont);
	ImGui::Text("Transform:");
	ImGui::PopFont();

	DrawVector3Edit("Position", Position, 0.0f, 0.001f);
	ImGui::Spacing();
	DrawVector3Edit("Rotation", Rotation, 0.0f, 0.1f);
	ImGui::Spacing();
	DrawVector3Edit("Scale", Scale, 1.0f, 0.01f);
	ImGui::End();
}

glm::mat4& Transform::GetModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, Position);

#pragma region Rotation Matrix Calculation
	glm::mat4 rotX = glm::mat4(1.0f);
	glm::mat4 rotY = glm::mat4(1.0f);
	glm::mat4 rotZ = glm::mat4(1.0f);

	float radX = glm::radians(Rotation.x);
	float sinX = sinf(radX);
	float cosX = cosf(radX);

	rotX[1][1] = cosX;
	rotX[2][1] = -sinX;
	rotX[1][2] = sinX;
	rotX[2][2] = cosX;

	float radY = glm::radians(Rotation.y);
	float sinY = sinf(radY);
	float cosY = cosf(radY);

	rotY[0][0] = cosY;
	rotY[2][0] = sinY;
	rotY[0][2] = -sinY;
	rotY[2][2] = cosY;

	float radZ = glm::radians(Rotation.z);
	float sinZ = sinf(radZ);
	float cosZ = cosf(radZ);

	rotZ[0][0] = cosZ;
	rotZ[1][0] = -sinZ;
	rotZ[0][1] = sinZ;
	rotZ[1][1] = cosZ;

	glm::mat4 rotationMatrix = rotX * rotY * rotZ;
#pragma endregion

	model *= rotationMatrix;
	model = glm::scale(model, Scale);

	return model;
}

void Transform::DrawVector3Edit(const std::string name, glm::vec3& data, float resetValue, float precision)
{
	// Credits for this function go to Yan Chernikov, a.k.a. 'The Cherno'
	// source: https://youtu.be/oESRecjuLNY
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::PushID(name.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70.0f);
	ImGui::Text(name.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 1));

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.81f, 0.1f, 0.15f, 1.0f));

	ImGui::PushFont(boldFont);
	if(ImGui::Button("X", buttonSize))
	{
		data.x = resetValue;
	}
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &data.x, precision, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));

	ImGui::PushFont(boldFont);
	if(ImGui::Button("Y", buttonSize))
	{
		data.y = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &data.y, precision, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));

	ImGui::PushFont(boldFont);
	if(ImGui::Button("Z", buttonSize))
	{
		data.z = resetValue;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &data.z, precision, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}