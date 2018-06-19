#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target);
	~Camera();
	void setPosition(glm::vec3 position);
	void setTarget(glm::vec3 target);
	void moveX(int verse);
	void moveY(int verse);
	void moveZ(int verse);
	void updateMouse(double xpos, double ypos);
	void Velocity(int verse);
	glm::mat4 getLookAt();
	glm::mat4 getProjection();
	void updateAspectRatio(unsigned int width, unsigned int height);

private:
	float Yaw = -90.0f, Pitch = 0.0f;
	double cameraSpeed = 0.5f, lastX, lastY;
	bool firstMouse = true;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 WorldUp;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::mat4 view;
	glm::mat4 projection;
	void updateLookAt();
};

