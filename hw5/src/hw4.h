#pragma once
#ifndef __HW4__
#define __HW4__
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"
#include "imgui.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class HW4
{
public:
	HW4();
	~HW4();
	void show_assignment(float width, float height);
	void use_gui();
	int assignment_num;
private:
	unsigned int VBO, VAO;
	Shader* shader;
	bool isDeepTest, rotating;
	int translation[3];
	float scale_size;
};

HW4::HW4() :isDeepTest(false), rotating(false), scale_size(1.0f)
{
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
	glBindBuffer(GL_ARRAY_BUFFER ,VBO);
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

HW4::~HW4()
{
	delete shader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

inline void HW4::show_assignment(float width, float height)
{
	static float angles = 0;
	if (isDeepTest) {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	if (rotating) {
		angles = (float)glfwGetTime();
	}
	model = glm::rotate(model, angles, glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));
	model = glm::translate(model, glm::vec3(translation[0], translation[1], translation[2]));

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

	projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);


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

inline void HW4::use_gui()
{
	ImGui::Begin("hw4");
	ImGui::Checkbox("Enable deep test", &isDeepTest);
	ImGui::Checkbox("Rotation", &rotating);
	ImGui::SliderInt3("Translation", translation, -10, 10);
	ImGui::SliderFloat("Scaling", &scale_size, 0.0f, 2.0f);
	ImGui::End();
}



#endif // !__HW4__
