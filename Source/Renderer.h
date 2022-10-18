#pragma once
#include <glad/glad.h>
#include <glfw3.h>

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

	void Setup();
	void Update();
	void Render();
};