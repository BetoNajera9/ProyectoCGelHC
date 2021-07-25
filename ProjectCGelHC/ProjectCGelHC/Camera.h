#pragma once

#include <glew.h>
#include <stdio.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	//Camaras
	GLboolean lock = GL_TRUE;
	GLboolean lastActionTAB = GL_FALSE;
	GLboolean lastActionSPACE = GL_FALSE;
	GLfloat ejeY = 10.0f;
	GLint NoCamera = 0;

	//Posicion
	glm::vec3 Entrada = glm::vec3(-3.6f, 10.0f, 1.52f);
	glm::vec3 Escalera1 = glm::vec3(-4.69f, 10.53f, -30.63f);
	glm::vec3 Principal = glm::vec3(34.22f, 14.48f, -15.74f);
	glm::vec3 Banios = glm::vec3(1.78f, 8.07f, -30.54f);
	glm::vec3 BanioH = glm::vec3(0.67f, 5.84f, -3.55f);
	glm::vec3 BanioM = glm::vec3(15.56f, 4.60f, -4.53f);
	glm::vec3 Escalera2 = glm::vec3(-5.0f, 22.21f, -30.56f);
	glm::vec3 Escalera3 = glm::vec3(43.21f, 21.92f, -29.07f);
	glm::vec3 Balcon = glm::vec3(11.24f, 21.5f, -1.42f);
	glm::vec3 Estacionamiento = glm::vec3(72.30f, 10.0f, -55.78f);
	//Direccion
	glm::vec3 EntradaD = glm::vec3(0.0f, -0.97f, 0.23f);
	glm::vec3 Escalera1D = glm::vec3(0.32f, -0.21f, 0.92f);
	glm::vec3 PrincipalD = glm::vec3(-0.99f, -0.038f, -0.0069f);
	glm::vec3 BaniosD = glm::vec3(0.94f, -0.32f, 0.072);
	glm::vec3 BanioHD = glm::vec3(0.57f, -0.32f, -0.57f);
	glm::vec3 BanioMD = glm::vec3(0.58f, -0.19, -0.78f);
	glm::vec3 Escalera2D = glm::vec3(0.57f, -0.54f, 0.61f);
	glm::vec3 Escalera3D = glm::vec3(-0.83, -0.54, -0.087);
	glm::vec3 BalconD = glm::vec3(0.13, -0.14, -0.98);

	void update();
};

