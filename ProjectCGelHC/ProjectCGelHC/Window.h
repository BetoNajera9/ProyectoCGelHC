#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();

	/*****Jhon*****/
	GLfloat getMoveJhonX() { return MoveJhonX; }
	GLfloat getMoveJhonY() { return MoveJhonY; }
	GLfloat getMoveJhonZ() { return MoveJhonZ; }

	GLfloat getRotateJhonX() { return RotateJhonX; }
	GLfloat getRotateJhonY() { return RotateJhonY; }
	GLfloat getRotateJhonZ() { return RotateJhonZ; }

	GLfloat getlastValueJhonX() { return lastValueJhonX; }
	GLfloat getlastValueJhonZ() { return lastValueJhonZ; }

	void setMoveJhonX(float X) { MoveJhonX = X; }
	void setMoveJhonY(float Y) { MoveJhonY = Y; }
	void setMoveJhonZ(float Z) { MoveJhonZ = Z; }

	void setRotateJhonX(float X) { RotateJhonX = X; }
	void setRotateJhonY(float Y) { RotateJhonY = Y; }
	void setRotateJhonZ(float Z) { RotateJhonZ = Z; }

	GLboolean getWalking() { return Walking; }
	/*************/

	//////////Luces//////////
	GLfloat getLuzBarra() { return LuzBarra; }
	GLfloat getLuzDj() { return LuzDj; }
	////////////////////////

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	//bool
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	/*****Jhon*****/
	GLboolean Walking;
	GLfloat MoveJhonX;
	GLfloat MoveJhonY;
	GLfloat MoveJhonZ;
	GLfloat RotateJhonX;
	GLfloat RotateJhonY;
	GLfloat RotateJhonZ;
	GLfloat lastValueJhonX;
	GLfloat lastValueJhonZ;
	/***************/
	//////////Luces//////////
	GLboolean LuzDj;
	GLboolean LuzBarra;
	/////////////////////////
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

