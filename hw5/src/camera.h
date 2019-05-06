#pragma once
#ifndef __CAMERA__
#define __CAMERA__
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "imgui.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera
{
public:
	Camera();
	~Camera();
	void moveForward(GLfloat const distance);
	void moveBack(GLfloat const distance);
	void moveRight(GLfloat const distance);
	void moveLeft(GLfloat const distance);
	void rotate(GLfloat const pitch, GLfloat const yaw);
	glm::mat4 getViewMat() const {
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

Camera::Camera() :cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f)
{
}

Camera::~Camera()
{
}

inline void Camera::moveForward(GLfloat const distance)
{
	cameraPos += distance * cameraFront;
}

inline void Camera::moveBack(GLfloat const distance)
{
	cameraPos -= distance * cameraFront;
}

inline void Camera::moveRight(GLfloat const distance)
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
}

inline void Camera::moveLeft(GLfloat const distance)
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
}

inline void Camera::rotate(GLfloat const pitch, GLfloat const yaw)
{
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}


#endif // !__CAMERA__
