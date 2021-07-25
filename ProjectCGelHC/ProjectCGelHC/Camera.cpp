#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_T])
	{
		if (lock == GL_FALSE) {
			position += front * velocity;
		}
		
	}

	if (keys[GLFW_KEY_G])
	{
		if (lock == GL_FALSE) {
			position -= front * velocity;
		}
		
	}

	if (keys[GLFW_KEY_F])
	{
		if (lock == GL_FALSE) {
			position -= right * velocity;
		}
		
	}

	if (keys[GLFW_KEY_H])
	{
		if (lock == GL_FALSE) {
			position += right * velocity;
		}
		
	}

	if (position.y > ejeY || position.y < ejeY) {
		position.y = ejeY;
	}

	if (keys[GLFW_KEY_SPACE]) {
		lastActionSPACE = GL_TRUE;
	}
	if (keys[GLFW_KEY_SPACE] == false && lastActionSPACE == GL_TRUE) {
		if (lock == GL_FALSE) {
			if (NoCamera == 0) {
				if (position != Entrada) {
					position = Entrada;
				}
				if (front != EntradaD) {
					front = EntradaD;
				}
			}
			else if (NoCamera == 1) {
				if (position != Escalera1) {
					position = Escalera1;
				}
				if (front != Escalera1D) {
					front = Escalera1D;
				}
			}
			else if (NoCamera == 2) {
				if (position != Principal) {
					position = Principal;
				}
				if (front != PrincipalD) {
					front = PrincipalD;
				}
			}
			else if (NoCamera == 3) {
				if (position != Banios) {
					position = Banios;
				}
				if (front != BaniosD) {
					front = BaniosD;
				}
			}
			else if (NoCamera == 4) {
				if (position != BanioH) {
					position = BanioH;
				}
				if (front != BanioHD) {
					front = BanioHD;
				}
			}
			else if (NoCamera == 5) {
				if (position != BanioM) {
					position = BanioM;
				}
				if (front != BanioMD) {
					front = BanioMD;
				}
			}
			else if (NoCamera == 6) {
				if (position != Escalera2) {
					position = Escalera2;
				}
				if (front != Escalera2D) {
					front = Escalera2D;
				}
			}
			else if (NoCamera == 7) {
				if (position != Escalera3) {
					position = Escalera3;
				}
				if (front != Escalera3D) {
					front = Escalera3D;
				}
			}
			else if (NoCamera == 8) {
				if (position != Balcon) {
					position = Balcon;
				}
				if (front != BalconD) {
					front = BalconD;
				}
			}
			else if (NoCamera == 9) {
				if (position != Estacionamiento) {
					position = Estacionamiento;
				}
				if (front != EntradaD) {
					front = EntradaD;
				}
			}

			lock = GL_TRUE;
		}
		else if (lock == GL_TRUE) {
			lock = GL_FALSE;
		}
		lastActionSPACE = GL_FALSE;
	}

	if (keys[GLFW_KEY_TAB]) {
		lastActionTAB = GL_TRUE;
	}
	if (keys[GLFW_KEY_TAB] == false && lastActionTAB == GL_TRUE) {
		if (NoCamera >= 9) {
			NoCamera = 0;
		}
		else {
			NoCamera += 1;
		}

		if (NoCamera == 0) {
			position = Entrada;
			front = EntradaD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 1) {
			position = Escalera1;
			front = Escalera1D;
			lock = GL_TRUE;
		}
		else if (NoCamera == 2) {
			position = Principal;
			front = PrincipalD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 3) {
			position = Banios;
			front = BaniosD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 4) {
			position = BanioH;
			front = BanioHD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 5) {
			position = BanioM;
			front = BanioMD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 6) {
			position = Escalera2;
			front = Escalera2D;
			lock = GL_TRUE;
		}
		else if (NoCamera == 7) {
			position = Escalera3;
			front = Escalera3D;
			lock = GL_TRUE;
		}
		else if (NoCamera == 8) {
			position = Balcon;
			front = BalconD;
			lock = GL_TRUE;
		}
		else if (NoCamera == 9) {
			position = Estacionamiento;
			front = EntradaD;
			lock = GL_TRUE;
		}
		ejeY = position.y;
		lastActionTAB = GL_FALSE;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
