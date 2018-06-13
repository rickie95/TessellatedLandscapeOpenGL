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
	glm::mat4 getLookAt();

private:
	float cameraSpeed = 0.01;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	glm::mat4 view;
	void updateLookAt();
};

