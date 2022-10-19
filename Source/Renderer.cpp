#include "Renderer.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

static void GLFWErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Renderer::Renderer()
{
	// Initialize GLFW // 
	glfwSetErrorCallback(GLFWErrorCallback);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "TinyPBR", NULL, NULL);
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1); // Enable vsync

	// Initialize GLAD // 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Error: Failed to initialize GLAD!\n");
		return;
	}

	glViewport(0, 0, windowWidth, windowHeight);

	// Initialize ImGui // 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Renderer::Run()
{
	Setup();

	static float deltaTime = 0.0f;
	static std::chrono::high_resolution_clock clock;
	static auto t0 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock.now())).time_since_epoch();

	while (isRunning)
	{
		auto t1 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock.now())).time_since_epoch();
		deltaTime = (t1 - t0).count() * .001;
		t0 = t1;

		StartFrame();
		Update(deltaTime);
		Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwWindowShouldClose(window))
		{
			isRunning = false;
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::Setup()
{
	model = new Model("Resources/Models/Helmet/Helmet.gltf");
	shaderProgram = new ShaderProgram("triangle.vert", "triangle.frag");
	camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::Update(float deltaTime)
{
	ProcessInput(window);
	camera->Update(window, deltaTime);
	camera->DebugDrawImGui();
	model->DebugDrawImGui();
}

void Renderer::Draw()
{
	shaderProgram->Bind();
	shaderProgram->SetMat4("VPMatrix", camera->GetViewProjectionMatrix());
	model->Draw(camera, shaderProgram);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}