#ifndef __HW3__
#define __HW3__
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"
#include "imgui.h"
#include <iostream>
#include <string>
using std::cout;
using std::endl;

class HW3
{
public:
	HW3();
	~HW3();
	void show_assignment(float point_size);
	void use_gui(bool show);
private:
	unsigned int VAO_GRID, VBO_GRID, VAO_POINT, VBO_POINT;
	void prepare_grid();
	void plot(float x, float y);
	void draw_triangle();
	void draw_circle();
	void plot_symmetry(int x, int y);
	void plotLineLow(int x0, int y0, int x1, int y1);
	void plotLineHigh(int x0, int y0, int x1, int y1);
	void plotLine(int x0, int y0, int x1, int y1);
	ImVec2 pos1, pos2, pos3;
	int x0, y0, x1, y1, x2, y2;
	int c_x, c_y, r;
	Shader *shader;
	int assignment_num;
};

HW3::HW3()
	:r(0), assignment_num(0)
{
	const char grid_shader_vertex[] =
		"#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 0.0f, 1.0f);\n"
		"}\n\0";
	const char grid_shader_fragment[] =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 our_color;\n"
		"void main()\n"
		"{\n"
		"	FragColor = our_color;\n"
		"}\n\0";
	
	shader = new Shader(grid_shader_vertex, grid_shader_fragment);

	glGenVertexArrays(1, &VAO_POINT);
	glGenBuffers(1, &VBO_POINT);
	// 初始化网格
	prepare_grid();
}

HW3::~HW3()
{
	delete shader;
	glDeleteVertexArrays(1, &VAO_GRID);
	glDeleteBuffers(1, &VBO_GRID);
	glDeleteVertexArrays(1, &VAO_POINT);
	glDeleteBuffers(1, &VBO_POINT);
}

inline void HW3::show_assignment(float point_size) {
	shader->use();
	std::string name = "our_color";
	int location = shader->getLocation(name);
	glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);
	glBindVertexArray(VAO_GRID);
	glDrawArrays(GL_LINES, 0, 88);
	glBindVertexArray(0);

	glPointSize(point_size / 2);
	glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
	if (!assignment_num) {
		draw_triangle();
	}
	else {
		draw_circle();
	}
	
}
inline void HW3::use_gui(bool show) {
	static int vertex0[2], vertex1[2], vertex2[2], circle_point[2];
	ImGui::Begin("hw3", &show);
	ImGui::Text("assignment 1");
	ImGui::SliderInt2("vertex1", vertex0, -5, 5);
	ImGui::SliderInt2("vertex2", vertex1, -5, 5);
	ImGui::SliderInt2("vertex3", vertex2, -5, 5);
	ImGui::Text("assignment 2");
	ImGui::SliderInt2("circle center", circle_point, -5, 5);
	ImGui::SliderInt("vertex3", &r, 0, 5);
	if (ImGui::CollapsingHeader("Select assignment")) {
		if (ImGui::Selectable("triangle", 0 == assignment_num)) {
			assignment_num = 0;
		}
		if(ImGui::Selectable("circle", 1 == assignment_num)) {
			assignment_num = 1;
		}
	}
	ImGui::End();
	x0 = vertex0[0];
	y0 = vertex0[1];
	x1 = vertex1[0];
	y1 = vertex1[1];
	x2 = vertex2[0];
	y2 = vertex2[1];
	c_x = circle_point[0];
	c_y = circle_point[1];
}

inline void HW3::prepare_grid()
{
	float line_pos[88];
	float y = 1.0f, interval = 0.2f;
	// 横向线条
	for (int i = 0; i < 44;)
	{
		/*
		x
		y
		*/
		line_pos[i++] = -1.0f;
		line_pos[i++] = y;
		line_pos[i++] = 1.0f;
		line_pos[i++] = y;
		y -= interval;
	}
	float x = -1.0f;
	for (int i = 44; i < 88;)
	{
		/*
		x
		y
		*/
		line_pos[i++] = x;
		line_pos[i++] = 1.0f;
		line_pos[i++] = x;
		line_pos[i++] = -1.0f;
		x += interval;
	}
	glGenVertexArrays(1, &VAO_GRID);
	glBindVertexArray(VAO_GRID);
	glGenBuffers(1, &VBO_GRID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_GRID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_pos), line_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline void HW3::plot(float x, float y)
{
	float pos[] = {
		x, y
	};

	glBindVertexArray(VAO_POINT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POINT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

inline void HW3::draw_triangle()
{
	plotLine(x0, y0, x1, y1);
	plotLine(x0, y0, x2, y2);
	plotLine(x2, y2, x1, y1);
}

inline void HW3::draw_circle()
{
	if (r == 0) return;
	int x = 0, y = r;
	int p = 3 - 2 * r;
	while (x <= y)
	{
		plot_symmetry(x, y);
		if (p < 0) {
			p = p + 4 * x + 6;
			x = x + 1;
		}
		else {
			p = p + 4 * (x - y) + 10;
			x = x + 1;
			y = y - 1;
		}
	}
}

inline void HW3::plot_symmetry(int x, int y)
{
	// 第1象限
	plot((float)x / 5 + (float)c_x / 5, (float)y / 5 + (float)c_y / 5);
	plot((float)y / 5 + (float)c_x / 5, (float)x / 5 + (float)c_y / 5);
	// 第2象限
	plot(-(float)x / 5 + (float)c_x / 5, (float)y / 5 + (float)c_y / 5);
	plot(-(float)y / 5 + (float)c_x / 5, (float)x / 5 + (float)c_y / 5);
	// 第3象限
	plot(-(float)x / 5 + (float)c_x / 5, -(float)y / 5 + (float)c_y / 5);
	plot(-(float)y / 5 + (float)c_x / 5, -(float)x / 5 + (float)c_y / 5);
	// 第4象限
	plot((float)x / 5 + (float)c_x / 5, -(float)y / 5 + (float)c_y / 5);
	plot((float)y / 5 + (float)c_x / 5, -(float)x / 5 + (float)c_y / 5);
}

inline void HW3::plotLineLow(int x0, int y0, int x1, int y1)
{
	int deta_x = x1 - x0, deta_y = y1 - y0, yi = 1;
	if (deta_y < 0) {
		yi = -1;
		deta_y = -deta_y;
	}
	int p = 2 * deta_y - deta_x, y = y0;
	
	for (int x = x0; x <= x1; x++)
	{
		plot(x / (float)5, y / (float)5);
		if (p > 0) {
			y = y + yi;
			p = p - 2 * deta_x;
		}
		p = p + 2 * deta_y;
	}
}

inline void HW3::plotLineHigh(int x0, int y0, int x1, int y1)
{
	int deta_x = x1 - x0, deta_y = y1 - y0, xi = 1;
	if (deta_x < 0) {
		xi = -1;
		deta_x = -deta_x;
	}
	int p = 2 * deta_x - deta_y, x = x0;

	for (int y = y0; y <= y1; y++)
	{
		plot(x / (float)5, y / (float)5);
		if (p > 0) {
			x = x + xi;
			p = p - 2 * deta_y;
		}
		p = p + 2 * deta_x;
	}
}

inline void HW3::plotLine(int x0, int y0, int x1, int y1)
{
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1)
			plotLineLow(x1, y1, x0, y0);
		else
			plotLineLow(x0, y0, x1, y1);
	}
	else {
		if (y0 > y1)
			plotLineHigh(x1, y1, x0, y0);
		else
			plotLineHigh(x0, y0, x1, y1);
	}
}

#endif // !__HW3__
