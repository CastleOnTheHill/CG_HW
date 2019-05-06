#pragma once
#ifndef __HW5__
#define __HW5__
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"
#include "imgui.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
class HW5
{
public:
	HW5();
	~HW5();
	void show_assignment(const float width, const float height, const Camera& camera);
	void use_gui(bool& cursorEnable);
private:
	unsigned int VBO, VAO;
	Shader* shader;
	float per_param[3][3];
	float ort_param[3][6];
	int selected;
	int assignment_num;
	int as1_num;
	bool test_what;
};

HW5::HW5() : selected(1), assignment_num(1), as1_num(1), test_what(false)
{
	per_param[0][0] = 45.0f;
	per_param[1][0] = 80.0f;
	per_param[2][0] = 45.0f;

	per_param[0][1] = 0.1f;
	per_param[1][1] = 0.1f;
	per_param[2][1] = 12.0f;

	per_param[0][2] = 100.0f;
	per_param[1][2] = 100.0f;
	per_param[2][2] = 100.0f;

	ort_param[0][0] = -80.0f;
	ort_param[0][1] = 80.0f;
	ort_param[0][2] = -80.0f;
	ort_param[0][3] = 80.0f;
	ort_param[0][4] = 0.1f;
	ort_param[0][5] = 100.0f;

	ort_param[1][0] = -30.0f;
	ort_param[1][1] = 30.0f;
	ort_param[1][2] = -30.0f;
	ort_param[1][3] = 30.0f;
	ort_param[1][4] = 0.1f;
	ort_param[1][5] = 100.0f;

	ort_param[2][0] = -10.0f;
	ort_param[2][1] = 10.0f;
	ort_param[2][2] = -10.0f;
	ort_param[2][3] = 10.0f;
	ort_param[2][4] = 12.0f;
	ort_param[2][5] = 100.0f;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	float vertices[] = {
		-2.0f, -2.0f, -2.0f,
		 2.0f, -2.0f, -2.0f,
		 2.0f,  2.0f, -2.0f,
		 2.0f,  2.0f, -2.0f,
		-2.0f,  2.0f, -2.0f,
		-2.0f, -2.0f, -2.0f,

		-2.0f, -2.0f,  2.0f,
		 2.0f, -2.0f,  2.0f,
		 2.0f,  2.0f,  2.0f,
		 2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f,  2.0f,
		-2.0f, -2.0f,  2.0f,

		-2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f, -2.0f,
		-2.0f, -2.0f, -2.0f,
		-2.0f, -2.0f, -2.0f,
		-2.0f, -2.0f,  2.0f,
		-2.0f,  2.0f,  2.0f,

		 2.0f,  2.0f,  2.0f,
		 2.0f,  2.0f, -2.0f,
		 2.0f, -2.0f, -2.0f,
		 2.0f, -2.0f, -2.0f,
		 2.0f, -2.0f,  2.0f,
		 2.0f,  2.0f,  2.0f,

		-2.0f, -2.0f, -2.0f,
		 2.0f, -2.0f, -2.0f,
		 2.0f, -2.0f,  2.0f,
		 2.0f, -2.0f,  2.0f,
		-2.0f, -2.0f,  2.0f,
		-2.0f, -2.0f, -2.0f,

		-2.0f,  2.0f, -2.0f,
		 2.0f,  2.0f, -2.0f,
		 2.0f,  2.0f,  2.0f,
		 2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f, -2.0f
	};
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//shader prepare
	const char fragmentShaderSrc[] =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec3 our_color ;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(our_color, 1.0);"
		"}\n\0";
	const char vertexShaderSrc[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n\0";
	shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
}

HW5::~HW5()
{
	delete shader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

inline void HW5::show_assignment(const float width, const float height, const Camera& camera)
{
	static float angles = 0;
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	
	if (assignment_num == 1) {
		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -1.5f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
		if (test_what) {
			projection = glm::ortho(ort_param[selected][0], ort_param[selected][1], ort_param[selected][2], ort_param[selected][3], ort_param[selected][4], ort_param[selected][5]);
		}
		else {
			projection = glm::perspective(glm::radians(per_param[selected][0]), width / height, per_param[selected][1], per_param[selected][2]);
		}
	}
	else if (assignment_num == 2) {
		float radius = 10.0f;
		float carmx = (float)sin(glfwGetTime()) * radius;
		float carmz = (float)cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(carmx, 0.0f, carmz), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(per_param[0][0]), width / height, per_param[0][1], per_param[0][2]);
	}
	else {
		view = camera.getViewMat();
		projection = glm::perspective(glm::radians(per_param[0][0]), width / height, per_param[0][1], per_param[0][2]);
	}

	

	glUniformMatrix4fv(shader->getLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(shader->getLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);

	glUniform3f(shader->getLocation("our_color"), 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUniform3f(shader->getLocation("our_color"), 1.0f, 0.5f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 6, 6);

	glUniform3f(shader->getLocation("our_color"), 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 12, 6);

	glUniform3f(shader->getLocation("our_color"), 0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 18, 6);

	glUniform3f(shader->getLocation("our_color"), 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 24, 6);

	glUniform3f(shader->getLocation("our_color"), 0.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 30, 6);
}

inline void HW5::use_gui(bool& cursorEnable)
{
	ImGui::Begin("hw5");
	ImGui::Checkbox("Test orthographic", &test_what);
	//ImGui::SliderInt3("Translation", translation, -10, 10);
	ImGui::Spacing();
	if (test_what) {
		if (ImGui::CollapsingHeader("orthographic projection")) {
			for (int n = 0; n < 3; n++)
			{
				char buf[100];
				sprintf(buf, "left %.1f, right %.1f, down %.1f, top %.1f, near %.1f, far %.1f", ort_param[n][0], ort_param[n][1], ort_param[n][2], ort_param[n][3], ort_param[n][4], ort_param[n][5]);
				if (ImGui::Selectable(buf, selected == n))
					selected = n;
			}
		}
	}
	else {
		if (ImGui::CollapsingHeader("perspective projection")) {
			for (int n = 0; n < 3; n++)
			{
				char buf[100];
				sprintf(buf, "angle %.1f, near %.1f, far %.1f", per_param[n][0], per_param[n][1], per_param[n][2]);
				if (ImGui::Selectable(buf, selected == n))
					selected = n;
			}
		}
	}
	if (ImGui::CollapsingHeader("Select assignment")) {
		if (ImGui::Selectable("assignment 1", 1 == assignment_num)) {
			assignment_num = 1;
		}
		if (ImGui::Selectable("assignment 2", 2 == assignment_num)) {
			assignment_num = 2;
		}
		if (ImGui::Selectable("bonus", 3 == assignment_num)) {
			if (assignment_num != 3) {
				cursorEnable = false;
			}
			assignment_num = 3;
		}
	}

	ImGui::End();
}



#endif // !__HW5__
