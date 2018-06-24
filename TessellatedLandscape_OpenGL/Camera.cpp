#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 target){
	this->position = position;
	this->target = target;
	lastX = 400;
	lastY = 300;
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)(800/600), 0.1f, 1000.0f);
	
	direction = glm::normalize(this->position - this->target);
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	updateLookAt();
}

Camera::~Camera(){
}

void Camera::setPosition(glm::vec3 position){
	this->position = position;
	updateLookAt();
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

void Camera::setTarget(glm::vec3 target){
	this->target = target;
	updateLookAt();
}

void Camera::moveX(int verse){
	glm::vec3 pos = this->position + verse*(float)cameraSpeed * Front;
	this->setPosition(pos);
}

void Camera::moveY(int verse){
	glm::vec3 pos = this->position + verse * (float)cameraSpeed * Up;
	this->setPosition(pos);
}

void Camera::moveZ(int verse){
	glm::vec3 pos = this->position + glm::normalize(glm::cross(Front, Up)) * ((float)cameraSpeed * verse);
	this->setPosition(pos);
}

void Camera::updateMouse(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = (float)(xpos - lastX);
	float yoffset = (float)(lastY - ypos);
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	this->updateLookAt();
}

void Camera::Velocity(int verse)
{
	double oldSpeed = log(cameraSpeed);
	this->cameraSpeed = pow(2.71, verse * 0.1 + oldSpeed);
	if (cameraSpeed < 0.01)
		cameraSpeed = 0.01;

	std::cout << cameraSpeed << std::endl;
}

glm::mat4 Camera::getLookAt() {
	
	return view;
}

glm::mat4 Camera::getProjection()
{
	return projection;
}

void Camera::updateAspectRatio(unsigned int width, unsigned int height)
{
	projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);
}

void Camera::updateLookAt(){

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
	view = glm::lookAt(position, position + Front, Up);
	/*
	direction = glm::normalize(position - target);
	Right = glm::normalize(glm::cross(up, direction));
	Up = glm::cross(direction, cameraRight);
	
	*/
}



