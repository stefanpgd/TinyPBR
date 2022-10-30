#include "Scene.h"
#include <imgui.h>

bool Scene::ChannelViewMode()
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];

	ImGui::BeginMainMenuBar();
	ImGui::Separator();

	ImGui::PushFont(boldFont);
	ImGui::Text("Channel/Texture View:");
	ImGui::PopFont();

	ImGui::PushItemWidth(130);
	if(ImGui::BeginCombo("##", channelViewTexts[currentChannel].c_str()))
	{
		for(int i = 0; i < channelViewTexts.size(); i++)
		{
			bool isSelected = (channelViewTexts[currentChannel] == channelViewTexts[i]);
			if(ImGui::Selectable(channelViewTexts[i].c_str(), isSelected))
			{
				currentChannel = i;
			}

			if(isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
	ImGui::EndMainMenuBar();



	if(currentChannel != 0)
	{
		return true;
	}

	return false;
}