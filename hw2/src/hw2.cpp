#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <iostream>

using std::cout;
using std::endl;

void framebuffer__size_callback(GLFWwindow* window, int width, int height);

int main() {

	int assignment_num = 4;

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
	
	float vertices1[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	float vertices2[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	float vertices3[] = {
		-1.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.0f
	};

	unsigned int index[] = {
		1, 3, 5,
		3, 0, 2,
		4, 0, 5
	};

	ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	Shader shader1("vertexShaderSrc2.txt", "fragmentShaderSrc2.txt");
	Shader shader2("vertexShaderSrc3.txt", "fragmentShaderSrc3.txt");
	Shader shader3("vertexShaderSrc3.txt", "fragmentShaderSrc3.txt");

	unsigned int VBO1, VAO1, VBO2, VAO2, VBO3, EBO3, VAO3;
	// assignment 1&2
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// assignment3
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// assignment5
	glGenVertexArrays(1, &VAO3);
	glBindVertexArray(VAO3);
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glGenBuffers(1, &EBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwMakeContextCurrent(window);
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		if (assignment_num < 3) {
			shader1.use();
			glBindVertexArray(VAO1);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else if (assignment_num == 3) {
			
			std::string name = "our_color";
			int location = shader2.getLocation(name);
			if (location == -1) {
				std::cout << "Failed to find " << name << endl;
			}
			else {
				shader2.use();
				glUniform4f(location, color.x, color.y, color.z, color.w);
			}
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else if (assignment_num == 4) {
			std::string name = "our_color";
			int location = shader3.getLocation(name);
			if (location == -1) {
				std::cout << "Failed to find " << name << endl;
			}
			else {
				shader3.use();
				glUniform4f(location, color.x, color.y, color.z, color.w);
			}
			glBindVertexArray(VAO3);
			glPointSize(10.0f);
			glDrawElements(GL_POINTS, 9, GL_UNSIGNED_INT, 0);
			glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
		}
		else {
			std::string name = "our_color";
			int location = shader3.getLocation(name);
			if (location == -1) {
				std::cout << "Failed to find " << name << endl;
			}
			else {
				shader3.use();
				glUniform4f(location, color.x, color.y, color.z, color.w);
			}
			glBindVertexArray(VAO3);
			glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		}
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		

		ImGui::Begin("color changer");
		ImGui::ColorEdit3("Editer  ", (float*)&color);
		if (ImGui::CollapsingHeader("Select assignment")) {
			for (int i = 1; i <= 5; i++)
			{
				char msg[32];
				sprintf(msg, "assignment %d", i);
				if (ImGui::Selectable(msg, i == assignment_num)) {
					assignment_num = i;
				}
			}
		}
		
		ImGui::End();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &VAO3);

	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);

	glDeleteBuffers(1, &EBO3);

	glfwTerminate();
	return 0;
}

void framebuffer__size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}
