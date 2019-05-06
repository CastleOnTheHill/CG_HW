#pragma once
#ifndef __HW2__
#define __HW2__

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"
#include "imgui.h"
#include <iostream>
using std::cout;
using std::endl;

class HW2
{
public:
	HW2();
	~HW2();
	void prepare_assignment();
	void show_assignment() const;
	void use_gui(bool show);
private:
	unsigned int VBO1, VAO1, VBO2, VAO2, VBO3, EBO3, VAO3;
	Shader* shader1, *shader2, *shader3;
	int assignment_num;
	ImVec4 color;
};

HW2::HW2()
	:color(0.0f, 0.0f, 0.0f, 1.0f)
{
	const char fragmentShaderSrc2[] =
		"#version 330 core\n"
		"in vec4 our_color;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = our_color;"
		"}\n\0";
	const char fragmentShaderSrc3[] = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 our_color;\n"
		"void main()\n"
		"{\n"
		"	FragColor = our_color;"
		"}\n\0";
	const char vertexShaderSrc2[] = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 color;\n"
		"out vec4 our_color;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"	our_color = vec4(color, 1.0f);\n"
		"}\n\0";
	const char vertexShaderSrc3[] =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"}\n\0";


	shader1 = new Shader(vertexShaderSrc2, fragmentShaderSrc2);
	shader2 = new Shader(vertexShaderSrc3, fragmentShaderSrc3);
	shader3 = new Shader(vertexShaderSrc3, fragmentShaderSrc3);
	assignment_num = 1;
}

HW2::~HW2()
{
	delete shader1;
	delete shader2;
	delete shader3;

	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &VAO3);

	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);

	glDeleteBuffers(1, &EBO3);
}

inline void HW2::prepare_assignment()
{
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

}

inline void HW2::show_assignment() const
{
	if (assignment_num < 3) {
		shader1->use();
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (assignment_num == 3) {

		std::string name = "our_color";
		int location = shader2->getLocation(name);
		if (location == -1) {
			std::cout << "Failed to find " << name << endl;
		}
		else {
			shader2->use();
			glUniform4f(location, color.x, color.y, color.z, color.w);
		}
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else if (assignment_num == 4) {
		std::string name = "our_color";
		int location = shader3->getLocation(name);
		if (location == -1) {
			std::cout << "Failed to find " << name << endl;
		}
		else {
			shader3->use();
			glUniform4f(location, color.x, color.y, color.z, color.w);
		}
		glBindVertexArray(VAO3);
		glPointSize(10.0f);
		glDrawElements(GL_POINTS, 9, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	}
	else {
		std::string name = "our_color";
		int location = shader3->getLocation(name);
		if (location == -1) {
			std::cout << "Failed to find " << name << endl;
		}
		else {
			shader3->use();
			glUniform4f(location, color.x, color.y, color.z, color.w);
		}
		glBindVertexArray(VAO3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	}
}

inline void HW2::use_gui(bool show)
{
	ImGui::Begin("hw2", &show);
	ImGui::ColorEdit3("color Editer  ", (float*)&color);
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
}



#endif // !__HW2__
