#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ShaderProgram;
class Camera;

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
	unsigned int VAO;

	void Setup();
	void StartFrame();
	void Update();
	void Draw();

	void ProcessInput(GLFWwindow* window);
};