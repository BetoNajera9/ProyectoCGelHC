#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

//Reloj
#include <iostream>
#include <chrono>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include"Model.h"
#include "Skybox.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"



const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture pisoTexture;

//Models declaration
Model Bar;
Model Bocina;
Model Barra;
Model Corona;
Model Laptop;
Model Lavabo;
Model Mesa;
Model MesaDj;
Model Mijitorio;
Model Silla;
Model Tornamesa;
Model WC;
Model Estacionamiento;
Model Auto;
Model Auto2;
Model Auto3;
Model Mueble;
Model Llanta_M;
Model Woman01;
Model Iroman;

//Personas
Model JhonBrazoDer;
Model JhonBrazoIzq;
Model JhonPiernaDer;
Model JhonPiernaIzq;
Model JhonTorzo;

// Varibles Jhon
bool Front = true;
bool Stopped = true;
float rotateJhonArmR;
float rotateJhonArmL;
float rotateJhonLegR;
float rotateJhonLegL;
float JhonX = 0.0f;
float JhonY = 0.0f;
float JhonZ = 0.0f;
float pisoJhon = 0;

//Tiempo
float timeStart, timeEnd;
bool Dia = true;

Skybox skybox;

float i = 0, j = 0;
bool up = true;
float posXpink, posZpink, posXblue, posZblue;

//materiales
Material Material_brillante;
Material Material_opaco;

// Lights
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];

SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(-3.6f, 10.0f, 1.52f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.4f);

	pisoTexture = Texture("Textures/piso.jpg");
	pisoTexture.LoadTextureA();

	// Models
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Bar = Model();
	Bar.LoadModel("Models/Bar.fbx");
	Barra = Model();
	Barra.LoadModel("Models/Barra.fbx");
	Bocina = Model();
	Bocina.LoadModel("Models/Bocina.fbx");
	Corona = Model();
	Corona.LoadModel("Models/Corona.obj");
	Laptop = Model();
	Laptop.LoadModel("Models/Laptop.obj");
	Lavabo = Model();
	Lavabo.LoadModel("Models/Lavabo.obj");
	Mesa = Model();
	Mesa.LoadModel("Models/Mesa.obj");
	MesaDj = Model();
	MesaDj.LoadModel("Models/MesaDJ.obj");
	Mijitorio = Model();
	Mijitorio.LoadModel("Models/Mijitorio.obj");
	Silla = Model();
	Silla.LoadModel("Models/Silla.fbx");
	Tornamesa = Model();
	Tornamesa.LoadModel("Models/Torna.obj");
	WC = Model();
	WC.LoadModel("Models/WC.obj");
	Estacionamiento = Model();
	Estacionamiento.LoadModel("Models/Estacionamiento.fbx");
	Auto = Model();
	Auto.LoadModel("Models/Auto.obj");
	Auto2 = Model();
	Auto2.LoadModel("Models/BMW.obj");
	Auto3 = Model();
	Auto3.LoadModel("Models/AutoBeto.obj");
	Mueble = Model();
	Mueble.LoadModel("Models/Mueble.obj");
	Woman01 = Model();
	Woman01.LoadModel("Models/Woman01.fbx");
	Iroman = Model();
	Iroman.LoadModel("Models/Iroman.obj");

	//Personas
	JhonBrazoDer = Model();
	JhonBrazoDer.LoadModel("Models/Jhon/JhonBrazoDer.fbx");
	JhonBrazoIzq = Model();
	JhonBrazoIzq.LoadModel("Models/Jhon/JhonBrazoIzq.fbx");
	JhonPiernaDer = Model();
	JhonPiernaDer.LoadModel("Models/Jhon/JhonPiernaDer.fbx");
	JhonPiernaIzq = Model();
	JhonPiernaIzq.LoadModel("Models/Jhon/JhonPiernaIzq.fbx");
	JhonTorzo = Model();
	JhonTorzo.LoadModel("Models/Jhon/JhonTorzo.fbx");

	std::vector<std::string> skyboxFacesNight;
	skyboxFacesNight.push_back("Textures/Skybox/NightCityR.jpg");
	skyboxFacesNight.push_back("Textures/Skybox/NightCityL.jpg");
	skyboxFacesNight.push_back("Textures/Skybox/NightCityD.jpg");
	skyboxFacesNight.push_back("Textures/Skybox/NightCityU.jpg");
	skyboxFacesNight.push_back("Textures/Skybox/NightCityF.jpg");
	skyboxFacesNight.push_back("Textures/Skybox/NightCityB.jpg");

	std::vector<std::string> skyboxFacesDay;
	skyboxFacesDay.push_back("Textures/Skybox/DayCityR.jpg");
	skyboxFacesDay.push_back("Textures/Skybox/DayCityL.jpg");
	skyboxFacesDay.push_back("Textures/Skybox/NightCityD.jpg");
	skyboxFacesDay.push_back("Textures/Skybox/DayCityU.jpg");
	skyboxFacesDay.push_back("Textures/Skybox/DayCityF.jpg");
	skyboxFacesDay.push_back("Textures/Skybox/DayCityB.jpg");

	skybox = Skybox(skyboxFacesDay);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//Lights
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	//Luz que van girar
	pointLights[0] = PointLight(1.0f, 0.0f, 1.0f, //Color
		9.0f, 9.0f,//intensidad
		15.0f, 7.0f, -15.0f,//Posicion
		0.3f, 0.2f, 0.1f);//4(.2)^2-2*0.5*0.1
	pointLightCount++;


	pointLights[1] = PointLight(0.0f, 1.0f, 1.0f, //Color
		9.0f, 9.0f,//intensidad
		25.0f, 7.0f, -15.0f,//Posicion
		0.3f, 0.2f, 0.1f);//4(.2)^2-2*0.5*0.1
	pointLightCount++;

	unsigned int spotLightCount = 0;


	//Luz Barras

	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f, //color
		1.0f, 2.0f,	//Intensidad
		22.4f, 80.0f, -3.5f,	//posicion
		//x     Y     Z
		0.0f, -5.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		3.5f);
	spotLightCount++;


////Luz blanca

spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, //color
	0.8f, 0.8f,	//Intensidad
	22.8f, 4.0f, -16.7f,	//posicion
	//x     Y     Z
	0.0f, -5.0f, 0.0f, //direccion
	1.0f, 0.0f, 0.0f,
	85.0f);
spotLightCount++;



//Luz Rosa
spotLights[2] = SpotLight(1.0f, 0.0f, 1.0f, //color
	0.8f, 0.8f,	//Intensidad
	7.4f, 4.0f, -16.7f,	//posicion
	//x     Y     Z
	0.0f, -5.0f, 0.0f, //direccion
	1.0f, 0.0f, 0.0f,
	85.0f);
spotLightCount++;

//Luz Mesa Dj
spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f, //color
	1.0f, 2.0f,	//Intensidad
	41.5f, 110.0f, -15.8f,	//posicion
	//x     Y     Z
	0.0f, -5.0f, 0.0f, //direccion
	1.0f, 0.0f, 0.0f,
	2.0f);
spotLightCount++;



//Luz Estacionamientos
spotLights[4] = SpotLight(1.0f, 1.0f, 1.0f, //color
	0.5f, 0.5f,	//Intensidad
	111.5f, 110.0f, -24.5f,	//posicion
	//x     Y     Z
	0.0f, -5.0f, 0.0f, //direccion
	1.0f, 0.0f, 0.0f,
	18.0f);
spotLightCount++;








GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;
glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);




timeStart = clock();

////Loop mientras no se cierra la ventana
while (!mainWindow.getShouldClose())
{
	GLfloat now = glfwGetTime();
	deltaTime = now - lastTime;
	deltaTime += (now - lastTime) / limitFPS;
	lastTime = now;

	//Recibir eventos del usuario
	glfwPollEvents();
	camera.keyControl(mainWindow.getsKeys(), deltaTime);
	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

	// Clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
	shaderList[0].UseShader();
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();

	//información en el shader de intensidad especular y brillo
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	/*===================Camara======================*/
	//printf("x:%f, y:%f, z:%f//", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	//printf("//Direccionx:%f, y:%f, z:%f\n", camera.getCameraDirection().x, camera.getCameraDirection().y, camera.getCameraDirection().z);
	/*===================+++++======================*/

	/******************Tiempo******************/
	timeEnd = clock();
	if ( ((float)timeEnd - timeStart) / CLOCKS_PER_SEC > 60 ){
		if (Dia == true) {
			Dia = false;
		}
		else if(Dia == false) {
			Dia = true;
		}
		timeStart = clock();
	}
	
	if (Dia == true) {
		mainLight.SetIntensity(0.8);
		skybox = Skybox(skyboxFacesDay);
	}
	else if (Dia == false) {
		mainLight.SetIntensity(0.05);
		skybox = Skybox(skyboxFacesNight);
	}

	//Espectaculo de luces//
	//Movimiento
	if (Dia == true) {
		pointLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		pointLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		spotLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		spotLights[2].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		spotLights[4].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	else if (Dia == false) {
		i -= 0.01;
		posXpink = 8 * cos(i);
		posZpink = 8 * sin(i);
		pointLights[0].SetPos(glm::vec3(15.0f + posXpink, 7.0f, -15.0f + posZpink));

		posZblue = 8 * cos(i);
		posXblue = 8 * sin(i);
		pointLights[1].SetPos(glm::vec3(15.0f + posXblue, 7.0f, -15.0f + posZblue));

		//Color
		if (up == true) {
			j += 0.005;
			if (j >= 1.0f) {
				up = false;
			}
		}
		else {
			j -= 0.005;
			if (j <= 0.0f) {
				up = true;
			}
		}
		pointLights[0].SetColor(glm::vec3(1.0f - j, 0.0f + j, 1.0f - j));
		pointLights[1].SetColor(glm::vec3(0.0f + j, 1.0f - j, 1.0f - j));

		//SpotLights
		spotLights[1].SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		spotLights[2].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
		spotLights[4].SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	// Luz con teclas
	//Luz Dj
	if (mainWindow.getLuzDj() == GL_TRUE) {
		spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (mainWindow.getLuzDj() == GL_FALSE) {
		spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	//Luz Barra
	if (mainWindow.getLuzBarra() == GL_TRUE) {
		spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (mainWindow.getLuzBarra() == GL_FALSE) {
		spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount);
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
	
	/*******************************************/



		glm::mat4 model(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bar.RenderModel();

		/**********************Jhon**********************/
		/*===========Recorrido===========*/
		//Paredes Afuera//
		//Enfrente
		if (mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonX() > -1.5f && mainWindow.getMoveJhonX() < 47.0f && mainWindow.getlastValueJhonZ() == 0.0f) {
			mainWindow.setMoveJhonZ(0.0f);
		}
		//Atras
		if (mainWindow.getMoveJhonZ() > -34.0f && mainWindow.getlastValueJhonZ() == -34.0f && mainWindow.getMoveJhonX() < 47.0f && mainWindow.getMoveJhonX() > -8.0f) {
			mainWindow.setMoveJhonZ(-34.0f);
		}
		//Derecha
		if (mainWindow.getMoveJhonX() < 47.0f && mainWindow.getlastValueJhonX() == 47.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonX() > -34.0f) {
			mainWindow.setMoveJhonX(47.0f);
		}
		//Izquierda
		if (mainWindow.getMoveJhonX() > -8.0f && mainWindow.getlastValueJhonX() == -8.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonX() > -34.0f) {
			mainWindow.setMoveJhonX(-8.0f);
		}
		///////////////////
		//Paredes Adentro//
		//Enfrente
		if (mainWindow.getMoveJhonZ() > -2.0f &&  mainWindow.getlastValueJhonZ() == -2.0f && mainWindow.getMoveJhonX() < 44.0f && mainWindow.getMoveJhonX() > -5.0f) {
			mainWindow.setMoveJhonZ(-2.0f);
		}
		//Atras
		if (mainWindow.getMoveJhonZ() < -31.0f && mainWindow.getlastValueJhonZ() == -31.0f && mainWindow.getMoveJhonX() < 44.0f && mainWindow.getMoveJhonX() > -5.0f) {
			mainWindow.setMoveJhonZ(-31.0f);
		}
		//Derecha
		if (mainWindow.getMoveJhonX() > 44.0f &&  mainWindow.getlastValueJhonX() == 44.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonZ() > -31.0f) {
			mainWindow.setMoveJhonX(44.0f);
		}
		//Izquierda
		if (mainWindow.getMoveJhonX() < -5.0f &&  mainWindow.getlastValueJhonX() == -5.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonZ() > -31.0f) {
			mainWindow.setMoveJhonX(-5.0f);
		}
		///////////////////
		/**********************************************/

		/***************Dentro del Bar*****************/
		if (mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonZ() >= -32.0f && mainWindow.getMoveJhonX() >= -6.0f && mainWindow.getMoveJhonX() < 46.0f) {
			// Corredor principal
			if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && mainWindow.getMoveJhonZ() >= -3.0f && pisoJhon == 0.5f) {
				pisoJhon = 0.0f;
				mainWindow.setMoveJhonY(0.25f);
			}
			else if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && mainWindow.getMoveJhonZ() <= -3.0f && pisoJhon == 0.0f) {
				pisoJhon = 0.5f;
			}

			if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && pisoJhon == 0.5) {
				// Escaleras principales
				if (mainWindow.getMoveJhonZ() <= -3.0f && mainWindow.getMoveJhonZ() >= -23.0f) {
					pisoJhon = 0.5;
					if (mainWindow.getlastValueJhonZ() > mainWindow.getMoveJhonZ() && mainWindow.getWalking() == GL_TRUE && pisoJhon == 0.5) {
						mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() + 0.15f);
					}
					else if (mainWindow.getlastValueJhonZ() < mainWindow.getMoveJhonZ() && mainWindow.getWalking() == GL_TRUE && pisoJhon == 0.5) {
						mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() - 0.15f);
					}
				}
				else if (mainWindow.getMoveJhonZ() <= -23.0f && pisoJhon == 0.5) {
					mainWindow.setMoveJhonY(9.2f);
					pisoJhon = 1.0f;
				}
				
			}
			// Bajar
			else if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && mainWindow.getMoveJhonZ() == -23 && mainWindow.getlastValueJhonZ() < mainWindow.getMoveJhonZ()) {
				pisoJhon = 0.5;
				mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() - 0.14f);
			}
			// Paredes del corredor
			if(mainWindow.getMoveJhonX() > -2.0f && mainWindow.getlastValueJhonX() == -2.0f && (pisoJhon == 0.5 || pisoJhon == 0.0f)){
				mainWindow.setMoveJhonX(-2.0f);

			}

			//1er piso
			if (pisoJhon == 1.0f) {
				/********Paredes********/
				//1era pared
				if (mainWindow.getMoveJhonX() > -2.0f &&  mainWindow.getlastValueJhonX() == -2.0f && mainWindow.getMoveJhonZ() < -26.0f && mainWindow.getMoveJhonZ() > -31.0f) {
					mainWindow.setMoveJhonX(-2.0f);
				}
				//2da
				if (mainWindow.getMoveJhonZ() < -26.0f &&  mainWindow.getlastValueJhonZ() == -26.0f && mainWindow.getMoveJhonX() > -2.0f && mainWindow.getMoveJhonX() < 29.0f) {
					mainWindow.setMoveJhonZ(-26.0f);
				}
				if (mainWindow.getMoveJhonZ() < -26.0f &&  mainWindow.getlastValueJhonZ() == -26.0f && mainWindow.getMoveJhonX() > 31.0f && mainWindow.getMoveJhonX() < 44.0f) {
					mainWindow.setMoveJhonZ(-26.0f);
				}
				//3ra
				if (mainWindow.getMoveJhonX() < 0.0f &&  mainWindow.getlastValueJhonX() == 0.0f && mainWindow.getMoveJhonZ() < -6.0f && mainWindow.getMoveJhonZ() > -23.0f) {
					mainWindow.setMoveJhonX(0.0f);
				}
			}
			//1ra Escalera
			if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= 0.0f && mainWindow.getMoveJhonZ() > -6.0f && pisoJhon == 1.0f) {
				pisoJhon = 1.5f;
			}
			if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && mainWindow.getMoveJhonZ() <= -6.0f && pisoJhon == 1.5f ) {
				pisoJhon = 1.5f;
				//Pared escaleras
				if (mainWindow.getMoveJhonX() > -2.0f &&  mainWindow.getlastValueJhonX() == -2.0f && mainWindow.getMoveJhonZ() < -6.0f && mainWindow.getMoveJhonZ() > -28.0f) {
					mainWindow.setMoveJhonX(-2.0f);
				}
			}
			else if (mainWindow.getMoveJhonX() >= 0.0f && mainWindow.getMoveJhonZ() >= -6.0f && pisoJhon == 1.5f) {
				pisoJhon = 1.0f;
			}

			//Subiendo escaleras
			if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && pisoJhon == 1.5) {
				if (mainWindow.getMoveJhonZ() <= -6.0f) {
					pisoJhon = 1.5f;
					//Pared escaleras
					if (mainWindow.getMoveJhonX() > -2.0f &&  mainWindow.getlastValueJhonX() == -2.0f && mainWindow.getMoveJhonZ() < -6.0f && mainWindow.getMoveJhonZ() > -28.0f) {
						mainWindow.setMoveJhonX(-2.0f);
					}
				}
				
				// Escaleras principales
				if (mainWindow.getMoveJhonZ() <= -6.0f && mainWindow.getMoveJhonZ() >= -28.0f && pisoJhon == 1.5f) {
					if (mainWindow.getlastValueJhonZ() > mainWindow.getMoveJhonZ() && mainWindow.getWalking() == GL_TRUE) {
						mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() + 0.12f);
					}
					else if (mainWindow.getlastValueJhonZ() < mainWindow.getMoveJhonZ() && mainWindow.getWalking() == GL_TRUE) {
						mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() - 0.12f);
					}
				}
				else if(mainWindow.getMoveJhonZ() <= -28.0f && pisoJhon == 1.5 && pisoJhon == 1.5f){
					mainWindow.setMoveJhonY(18.0f);
					pisoJhon = 2;
				}
			}
			// Bajar
			else if (mainWindow.getMoveJhonX() >= -5.0f && mainWindow.getMoveJhonX() <= -2.0f && mainWindow.getMoveJhonZ() == -28.0f && mainWindow.getlastValueJhonZ() < mainWindow.getMoveJhonZ() && pisoJhon == 2.0f) {
				pisoJhon = 1.5;
				mainWindow.setMoveJhonY(mainWindow.getMoveJhonY() - 0.14f);
			}

			//2do Piso
			if (pisoJhon == 2) {
				//Escaleras 1
				if (mainWindow.getMoveJhonZ() < -13.0f &&  mainWindow.getlastValueJhonZ() == -13.0f && mainWindow.getMoveJhonX() > -5.0f && mainWindow.getMoveJhonX() < 0.0f) {
					mainWindow.setMoveJhonZ(-13.0f);
				}
				if (mainWindow.getMoveJhonX() < 0.0f &&  mainWindow.getlastValueJhonX() == 0.0f && mainWindow.getMoveJhonZ() > -28.0f && mainWindow.getMoveJhonZ() < -13.0f) {
					mainWindow.setMoveJhonX(0.0f);
				}
				//Escaleras 2
				if (mainWindow.getMoveJhonZ() < -26.0f &&  mainWindow.getlastValueJhonZ() == -26.0f && mainWindow.getMoveJhonX() > 25.0f && mainWindow.getMoveJhonX() < 41.0f) {
					mainWindow.setMoveJhonZ(-26.0f);
				}
				if (mainWindow.getMoveJhonX() > 25.0f &&  mainWindow.getlastValueJhonX() == 25.0f && mainWindow.getMoveJhonZ() > -31.0f && mainWindow.getMoveJhonZ() < -26.0f) {
					mainWindow.setMoveJhonX(25.0f);
				}

				//////Mesa DJ///////
				//Enfrente
				if (mainWindow.getMoveJhonZ() < -12.0f &&  mainWindow.getlastValueJhonZ() == -12.0f && mainWindow.getMoveJhonX() > 39.5f && mainWindow.getMoveJhonX() < 43.5f) {
					mainWindow.setMoveJhonZ(-12.0f);
				}
				//Atras
				if (mainWindow.getMoveJhonZ() > -20.0f &&  mainWindow.getlastValueJhonZ() == -20.0f && mainWindow.getMoveJhonX() > 39.5f && mainWindow.getMoveJhonX() < 43.5f) {
					mainWindow.setMoveJhonZ(-20.0f);
				}
				//Derecha
				if (mainWindow.getMoveJhonX() < 43.5f &&  mainWindow.getlastValueJhonX() == 43.5f && mainWindow.getMoveJhonZ() > -20.0f && mainWindow.getMoveJhonZ() < -12.0f) {
					mainWindow.setMoveJhonX(43.5f);
				}
				//Izquierda
				if (mainWindow.getMoveJhonX() > 39.5f &&  mainWindow.getlastValueJhonX() == 39.5f && mainWindow.getMoveJhonZ() > -20.0f && mainWindow.getMoveJhonZ() < -12.0f) {
					mainWindow.setMoveJhonX(39.5f);
				}
				////////////////////

				//Balcon//
				//Enfrente
				if (mainWindow.getMoveJhonZ() < -9.0f &&  mainWindow.getlastValueJhonZ() == -9.0f && mainWindow.getMoveJhonX() > 3.0f && mainWindow.getMoveJhonX() < 36.0f) {
					mainWindow.setMoveJhonZ(-9.0f);
				}
				//Atras
				if (mainWindow.getMoveJhonZ() > -24.0f &&  mainWindow.getlastValueJhonZ() == -24.0f && mainWindow.getMoveJhonX() > 3.0f && mainWindow.getMoveJhonX() < 36.0f) {
					mainWindow.setMoveJhonZ(-24.0f);
				}
				//Derecha
				if (mainWindow.getMoveJhonX() < 36.0f &&  mainWindow.getlastValueJhonX() == 36.0f && mainWindow.getMoveJhonZ() > -24.0f && mainWindow.getMoveJhonZ() < -9.0f) {
					mainWindow.setMoveJhonX(36.0f);
				}
				//Izquierda
				if (mainWindow.getMoveJhonX() > 3.0f &&  mainWindow.getlastValueJhonX() == 3.0f && mainWindow.getMoveJhonZ() > -24.0f && mainWindow.getMoveJhonZ() < -9.0f) {
					mainWindow.setMoveJhonX(3.0f);
				}
				//////////

				//Enfrente
				if (mainWindow.getMoveJhonZ() > -2.0f &&  mainWindow.getlastValueJhonZ() == -2.0f && mainWindow.getMoveJhonX() < 44.0f && mainWindow.getMoveJhonX() > -5.0f) {
					mainWindow.setMoveJhonZ(-2.0f);
				}
				//Atras
				if (mainWindow.getMoveJhonZ() < -31.0f && mainWindow.getlastValueJhonZ() == -31.0f && mainWindow.getMoveJhonX() < 44.0f && mainWindow.getMoveJhonX() > -5.0f) {
					mainWindow.setMoveJhonZ(-31.0f);
				}
				//Derecha
				if (mainWindow.getMoveJhonX() > 44.0f &&  mainWindow.getlastValueJhonX() == 44.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonZ() > -31.0f) {
					mainWindow.setMoveJhonX(44.0f);
				}
				//Izquierda
				if (mainWindow.getMoveJhonX() < -5.0f &&  mainWindow.getlastValueJhonX() == -5.0f && mainWindow.getMoveJhonZ() < 0.0f && mainWindow.getMoveJhonZ() > -31.0f) {
					mainWindow.setMoveJhonX(-5.0f);
				}
			}
			
		}
		else {
			mainWindow.setMoveJhonY(0.0f);
		}

		/*=================================*/

		/*if (mainWindow.getWalking() == GL_TRUE) {
			if (Front == true) {
				rotateJhonArmR += 0.5;
				rotateJhonArmL -= 0.5;
				if (rotateJhonArmR == -45) {
					Front = false;
				}
			}
			else if (Front == false) {
				rotateJhonArmR -= 0.5;
				rotateJhonArmL += 0.5;
				if (rotateJhonArmR == 45) {
					Front = true;
				}
			}
		}
		else {
			if (Stopped == false && rotateJhonArmR > 0) {
				rotateJhonArmR -= 0.5;
				rotateJhonArmL += 0.5;
			}
			if (Stopped == false && rotateJhonArmR < 0) {
				rotateJhonArmR += 0.5;
				rotateJhonArmL -= 0.5;
			}
		}*/
		
		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMoveJhonX(), 0.3f + mainWindow.getMoveJhonY(), 0.0f + mainWindow.getMoveJhonZ()));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotateJhonZ() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 25 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotateJhonArmR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JhonBrazoDer.RenderModel();

		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMoveJhonX(), 0.3f + mainWindow.getMoveJhonY(), 0.0f + mainWindow.getMoveJhonZ()));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotateJhonZ() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotateJhonArmL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JhonBrazoIzq.RenderModel();

		if (mainWindow.getWalking() == GL_TRUE) {
			if (Front == true) {
				rotateJhonLegR -= 2.0;
				rotateJhonLegL += 2.0;
				if (rotateJhonLegR == -30) {
					Front = false;
				}
			}
			else if (Front == false) {
				rotateJhonLegR += 2.0;
				rotateJhonLegL -= 2.0;
				if (rotateJhonLegR == 30) {
					Front = true;
				}
			}
		}
		else {
			if (rotateJhonLegR == 0) {
				Stopped = true;
			}
			else {
				Stopped = false;
			}

			if (Stopped == false && rotateJhonLegR > 0) {
				rotateJhonLegR -= 1.5;
				rotateJhonLegL += 1.5;
			}
			if (Stopped == false && rotateJhonLegR < 0) {
				rotateJhonLegR += 1.5;
				rotateJhonLegL -= 1.5;
			}
		}
		
		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMoveJhonX(), 0.1f + mainWindow.getMoveJhonY(), 0.0f + mainWindow.getMoveJhonZ()));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotateJhonZ() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotateJhonLegR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JhonPiernaDer.RenderModel();

		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMoveJhonX(), 0.1f + mainWindow.getMoveJhonY(), 0.0f + mainWindow.getMoveJhonZ()));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotateJhonZ() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotateJhonLegL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JhonPiernaIzq.RenderModel();

		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getMoveJhonX(), 0.0f + mainWindow.getMoveJhonY(), 0.0f + mainWindow.getMoveJhonZ()));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, mainWindow.getRotateJhonZ() * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JhonTorzo.RenderModel();
		//****************************  PLANTA BAJA ********************************


		 //BAÑOS HOMBRES

		//Lavabo1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-0.05f, -2.8f, -8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();

		//Lavabo2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-0.05f, -2.8f, -16.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();

		//Lavabo3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-0.05f, -2.8f, -24.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();
		//Lavabo4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(-0.05f, -2.8f, -32.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();
		//WC1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.0f, -0.7f, -10.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.0f, -0.7f, -8.3f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.0f, -0.7f, -6.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//Mijitorio1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.0f, -1.0f, -8.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mijitorio.RenderModel();


		//Mijitorio2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.0f, -1.0f, -5.2f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mijitorio.RenderModel();

		//Mijitorio3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.0f, -1.0f, -2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mijitorio.RenderModel();



										//BAÑOS MUJERES


		//Lavabo1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(21.8f, -2.8f, -8.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();

		//Lavabo2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(21.8f, -2.8f, -16.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();


		//Lavabo3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(21.8f, -2.8f, -24.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();

		//Lavabo4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::translate(model, glm::vec3(21.8f, -2.8f, -32.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lavabo.RenderModel();


		//WC1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1f, -0.7f, -10.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1, -0.7f, -8.3f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1f, -0.7f, -6.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1f, -0.7f, -4.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC5
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1f, -0.7f, -2.7f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();

		//WC6
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(11.1f, -0.7f, -1.1f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WC.RenderModel();





		//****************************   PRIMER PISO   *********************************

		//Mueble
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(20.0f, 7.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mueble.RenderModel();

		//Botellas en el mueble.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 17.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 17.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 17.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 17.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();








		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 17.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 17.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 17.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 17.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();














		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 18.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 18.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 18.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 18.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();







		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 18.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 18.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 18.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 18.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();







		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 18.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 18.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 18.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 18.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();







		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 19.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 19.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 19.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 19.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();




		//Barra
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(14.5f, 4.5f, -3.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barra.RenderModel();






		//Bocina1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(86.0f, 24.8f, -3.6f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();

		//Bocina2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(88.0f, 23.0f, -49.6f));
		model = glm::rotate(model, 130 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();

		//Bocina3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(-1.0f, 24.8f, -5.8f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();







		//Conjunto de Mesa 1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(7.5f, 5.4f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(7.1f, 4.7f, -2.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(6.0f, 4.7f, -2.5f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();





		//Conjunto de Mesa 2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(11.5f, 5.4f, -7.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(10.5f, 4.7f, -6.1f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.5f, 4.7f, -6.1f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();




		//Conjunto de Mesa 3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(17.5f, 5.4f, -7.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(15.7f, 4.7f, -6.1f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(14.6f, 4.7f, -6.1f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();



		//Conjunto de Mesa 4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(24.5f, 5.4f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(21.5f, 4.7f, -2.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(20.6f, 4.7f, -2.5f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();






		//Conjunto de Mesa 5
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(27.5f, 5.4f, -7.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(24.5f, 4.7f, -6.1f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(23.2f, 4.7f, -6.1f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();





		//Conjunto de Mesa 6
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(27.5f, 5.4f, -17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(24.5f, 4.7f, -14.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(23.2f, 4.7f, -14.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();


		//Conjunto de Mesa 7
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(17.5f, 5.4f, -17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(15.7f, 4.7f, -14.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(14.6f, 4.7f, -14.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();




		//Conjunto de Mesa 8
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(11.5f, 5.4f, -17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(10.5f, 4.7f, -14.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.5f, 4.7f, -14.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();






		//**************************** SEGUNDO PISO *********************************


		//Mueble
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(20.0f, 16.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mueble.RenderModel();





		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 10.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 10.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 10.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 10.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();








		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 9.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 9.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 9.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 9.9f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();









		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 9.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 9.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 9.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 9.4f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();







		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 9.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 9.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 9.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 9.0f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();










		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(22.5f, 8.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.9f, 8.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(21.3f, 8.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.7f, 8.5f, -1.1f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Corona.RenderModel();



		//Barra
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(14.5f, 10.8f, -3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Barra.RenderModel();


		//Mesa DJ
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.7f, 3.0f, 1.7f));
		model = glm::translate(model, glm::vec3(24.6f, 5.3f, -8.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MesaDj.RenderModel();


		//Laptop
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(28.5f, 11.05f, -9.4f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Laptop.RenderModel();



		//Tornamesa
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(27.7f, 11.9f, -11.2));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tornamesa.RenderModel();


		//Bocina1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(86.0f, 41.8f, -3.6f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();


		//Bocina2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(88.0f, 41.8f, -60.6f));
		model = glm::rotate(model, 130 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();

		//Bocina3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(-11.0f, 41.8f, -5.8f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina.RenderModel();



		//Conjunto de Mesa 1
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(1.7f, 12.5f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(2.1f, 10.9f, -2.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(1.0f, 10.9f, -2.5f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();



		//Conjunto de Mesa 2
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(7.5f, 12.3f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(7.1f, 10.7f, -2.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(6.0f, 10.7f, -2.5f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();



		//Conjunto de Mesa 3
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(24.3f, 12.3f, -3.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(21.5f, 10.7f, -2.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(20.6f, 10.7f, -2.5f));
		model = glm::rotate(model, 85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();




		//Conjunto de Mesa 4
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(17.5f, 12.3f, -22.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(15.7f, 10.7f, -19.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(14.6f, 10.7f, -19.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();





		//Conjunto de Mesa 5
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(11.7f, 12.3f, -22.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(10.7f, 10.7f, -19.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(9.6f, 10.7f, -19.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();







		//Conjunto de Mesa 6
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::translate(model, glm::vec3(5.8f, 12.3f, -22.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.RenderModel();
		//sillas
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(5.4f, 10.7f, -19.8f));
		model = glm::rotate(model, -110 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(4.6f, 10.7f, -19.8f));
		model = glm::rotate(model, 50 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla.RenderModel();



//***************************** Estacinamiento ***********************************




		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 20.0f, -60.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estacionamiento.RenderModel();



        //Carro 1.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(139.0f, 0.2f, -10.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(134.0f, 0.0f, -9.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(134.0f, 0.0f, -15.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(125.5f, 0.0f, -9.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(125.5f, 0.0f, -15.7f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f, 0.015f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		//Carro 2.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.5f, 0.2f, -28.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto2.RenderModel();


		//Carro 3.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(152.8f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Auto3.RenderModel();















		//Personas

		//woman
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(152.8f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Woman01.RenderModel();

		//iroman
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(152.8f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(-1.0f, -1.0f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Iroman.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
