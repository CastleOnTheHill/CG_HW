#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <iostream>
#include "hw2.h"
#include "hw3.h"
#include "hw4.h"
#include "hw5.h"

using std::cout;
using std::endl;

void framebuffer__size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
int width, height;
float yaw, pitch;
bool firstMouse = true;
bool cursorEnable = true;
Camera camera;

int main() {
	int hw_num = 2;
	yaw = 0;
	pitch = 0;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "hw", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLF window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	const char* glsl_version = "#version 330";
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsClassic();

	
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer__size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	HW2 hw2;
	HW3 hw3;
	HW4 hw4;
	HW5 hw5;
	hw2.prepare_assignment();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	
	while (!glfwWindowShouldClose(window))
	{
		if (cursorEnable) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		processInput(window);
		glfwPollEvents();
		glfwMakeContextCurrent(window);
		glfwGetWindowSize(window, &width, &height);
		float point_size = width < height ? width / (float)10 : height / (float)10;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("chose homework"))
			{
				if (ImGui::MenuItem("hw2")) {
					hw_num = 2;
				}
				if (ImGui::MenuItem("hw3")) {
					hw_num = 3;
				}
				if (ImGui::MenuItem("hw4")) {
					hw_num = 4;
				}
				if (ImGui::MenuItem("hw5")) {
					hw_num = 5;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		switch (hw_num)
		{
		case 2:
			hw2.show_assignment();
			hw2.use_gui(true);
			break;
		case 3:
			hw3.show_assignment(point_size);
			hw3.use_gui(true);
			break;
		case 4:
			hw4.show_assignment((float)width, (float)height);
			hw4.use_gui();
			break;
		case 5:
			hw5.show_assignment((float)width, (float)height, camera);
			hw5.use_gui(cursorEnable);
			break;
		default:
			break;
		}
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void framebuffer__size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		cursorEnable = true;
	}

	deltaTime = (float)glfwGetTime() - lastFrame;
	lastFrame = (float)glfwGetTime();
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBack(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(cameraSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static float lastX = (float)width / 2;
	static float lastY = (float)height / 2;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	camera.rotate(pitch, yaw);
}
