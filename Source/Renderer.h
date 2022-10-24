#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ShaderProgram;
class Camera;
class AModel;

class Renderer
{
public:
	Renderer();

	void Run();

private:
	bool isRunning = true;
	GLFWwindow* window;
	unsigned int windowWidth = 1080;
	unsigned int windowHeight = 720;

	Camera* camera;
	ShaderProgram* shaderProgram;
	AModel* model;

	void Setup();
	void StartFrame();
	void Update(float deltaTime);
	void Draw();

	void ProcessInput(GLFWwindow* window);
	void TakeScreenshot(std::string filePath);
};