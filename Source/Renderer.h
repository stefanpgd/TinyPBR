#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ShaderProgram;
class Camera;
class Model;
class SceneManager;

class Renderer
{
public:
	Renderer();

	void Run();

private:
	bool isRunning = true;
	unsigned int windowWidth = 1920;
	unsigned int windowHeight = 1080;

	Camera* camera;
	SceneManager* sceneManager;
	GLFWwindow* window;

	void Setup();
	void StartFrame();
	void Update(float deltaTime);
	void Draw();

	void ProcessInput(GLFWwindow* window);
	void TakeScreenshot(std::string filePath);
};