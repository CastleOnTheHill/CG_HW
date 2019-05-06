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

using std::cout;
using std::endl;

void framebuffer__size_callback(GLFWwindow* window, int width, int height);

int main() {
	int hw_num = 2;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "hw2", NULL, NULL);
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
	

	HW2 hw2;
	hw2.prepare_assignment();

	HW3 hw3;
	HW4 hw4;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	int width, height;
	while (!glfwWindowShouldClose(window))
	{
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
