#include <fstream>
#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include "assimp/BaseImporter.h"


#define WIDTH 800
#define HEIGHT 600

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


void init(GLFWwindow* window) {
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Model Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "GLEW error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	init(window);

	VertexArray va;
	va.bind();

	float vertices[] = {
		// positions		  // texture coordinates
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	// front bottom right
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front bottom left
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	// front top left

		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	// front top left
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	// front top right
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	// front bottom right

		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	// back bottom right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	// back bottom left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	// back top left
		
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	// back top left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	// back bottom right

		 //

		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	// front top right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front bottom right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top right

		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top right
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front bottom right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	// back bottom right

		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	// front top left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front bottom left
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top left
		 
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front bottom left
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	// back bottom left

		//

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	// front top left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	// front top right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	// back top left

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	// back top left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	// front top right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	// back top right

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	// front top left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	// front top right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	// back top left
		
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	// back top left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	// front top right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f	// back top right
	};

	// complete the textures
	Texture brickTexture(GL_TEXTURE_2D, R"(src\res\wall.jpg)");

	VertexBuffer vb(vertices, sizeof(vertices));
	vb.addLayout(0, 3);
	vb.addLayout(1, 2);
	vb.setLayout();

	va.unBind();

	Shader basic(R"(src\shaders/basic.glsl)");

	glEnable(GL_DEPTH_TEST);

	ImVec4 boxColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	float scale = 1.0f;
	glm::vec3 rotation(0.0f, 0.0f, 0.0f);


	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		glm::mat4 model = glm::mat4(1.0f);
		{
			ImGui::Begin("Hello, world!");
			ImGui::ColorEdit3("Box Color", (float*)&boxColor);

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			ImGui::SliderFloat("Scale", &scale, 0.2, 2, "%.5f", 0);
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			ImGui::SliderFloat3("Rotation", glm::value_ptr(rotation), 0, 360, "%.2f", 0);
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}


		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		
		va.bind();
		// use program here
		basic.bind();
		basic.setUniform("Color", boxColor.x, boxColor.y, boxColor.z, boxColor.w);
		basic.setUniform("model", model);
		basic.setUniform("projection", projection);
		basic.setUniform("view", view);
		brickTexture.bind();
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		basic.unBind();
		brickTexture.unBind();
		va.unBind();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // regain control over cursor

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(yOffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
