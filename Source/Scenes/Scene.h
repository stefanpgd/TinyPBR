#pragma once
#include <vector>
#include <string>

class ShaderProgram;
class Model;
class Camera;

enum class ChannelViewMode
{
	All,
	Albedo,
	Normal,
	Metallic,
	Roughness,
	AO
};

class Scene
{
public:
	virtual void Update() = 0;
	virtual void Draw(Camera* camera) = 0;

protected:
	bool ChannelViewMode();

	std::vector<Model*> models;
	ShaderProgram* shaderProgram;
	ShaderProgram* channelViewProgram;
	unsigned int currentChannel;

private:
	std::vector<std::string> channelViewTexts
	{
		"All",
		"Albedo",
		"Normal",
		"Metallic",
		"Roughness",
		"Ambient Occlusion"
	};
};