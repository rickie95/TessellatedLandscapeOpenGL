#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target){
	this->position = position;
	this->target = target;
	direction = glm::normalize(position - target);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, direction));
	cameraUp = glm::cross(direction, cameraRight);

	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	view = glm::lookAt(position, position + cameraFront, cameraUp);
}

Camera::~Camera(){
}

void Camera::setPosition(glm::vec3 position){
	this->position = position;
	updateLookAt();
}

void Camera::setTarget(glm::vec3 target){
	this->target = target;
	updateLookAt();
}

void Camera::moveX(int verse){
	glm::vec3 pos = this->position + verse*cameraSpeed * cameraFront;
	this->setPosition(pos);
}

void Camera::moveY(int verse){
	glm::vec3 pos = this->position + verse * cameraSpeed * cameraFront;
	this->setPosition(pos);
}

void Camera::moveZ(int verse){
	glm::vec3 pos = this->position + glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * verse);
	this->setPosition(pos);
}

glm::mat4 Camera::getLookAt() {
	return view;
}

void Camera::updateLookAt(){
	direction = glm::normalize(position - target);
	cameraRight = glm::normalize(glm::cross(up, direction));
	cameraUp = glm::cross(direction, cameraRight);
	view = glm::lookAt(position, position + cameraFront, cameraUp);
}


