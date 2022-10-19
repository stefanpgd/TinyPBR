#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer();

	void Run();

private:
	bool isRunning = true;
	GLFWwindow* window;
	unsigned int screenWidth = 1080;
	unsigned int screenHeight = 720;

	void StartFrame();
	void Update();
	void Render();

	void ProcessInput(GLFWwindow* window);
};