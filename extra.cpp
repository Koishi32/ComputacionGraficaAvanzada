#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliHeliBack;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Modelos animados

//Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLamp2Post;

//New Lamp

Model modelLampPlant2;

// Mayow
Model mayowModelAnimate;
// Cowboy
Model cowboyModelAnimate;
// Guardian con lampara
Model guardianModelAnimate;
// Cybog
Model cyborgModelAnimate;
// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightmap.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
glm::mat4 modelMatrixMayow = glm::mat4(1.0f);
glm::mat4 modelMatrixCowboy = glm::mat4(1.0f);
glm::mat4 modelMatrixGuardian = glm::mat4(1.0f);
glm::mat4 modelMatrixCyborg = glm::mat4(1.0f);

int animationMayowIndex = 1;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftArm = 0.0, rotBuzzLeftForeArm = 0.0, rotBuzzLeftHand = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 100;
int numPasosBuzz = 0;

//Lamp position
std::vector<glm::vec3> lamp1Position = {
	glm::vec3(-7.03,0,-19.14),
	glm::vec3(24.41,0,-34.57),
	glm::vec3(-10.15,0,-54.1),
	//glm::vec3()
};

std::vector<float> lamp1Orientation={
	-17.0,-82.67,23.70
};
std::vector<std::pair<glm::vec3,float>> lamp2PosOri = {
	{glm::vec3(-36.52,0,-23.24) , 21.37+90},
	{glm::vec3(-52.73,0,-3.90), -65.0 +90},
	{glm::vec3(49.6,0,-26.9),25.0+90}
};
// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelBack = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

double deltaTime;
double currTime, lastTime;

// Variables animacion maquina de estados eclipse
const float avance = 0.1;
const float giroEclipse = 0.5f;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void generatePointLight(glm::vec3 LampPlantPostion2,int i,glm::vec3 DiffuseColor,glm::vec3 ambient,glm::vec3 specular,float quadratic);
void cars_behavoir();
void DoorsLambo();
// Transform to OpenGL Coordenates
glm::vec3 TransformGIMPCoordenatesToOpenGLPixels(glm::vec3 coordenadas);

glm::vec3 TransformGIMPCoordenatesToOpenGLPixels(glm::vec3 coordenadas){
    glm::vec3 resultado = glm::vec3(0,0,0);
    resultado.x = (coordenadas.x - 256) / 2.56;
    resultado.y = 0;
    resultado.z = (coordenadas.z -256) / 2.56;
    return resultado;
}
std::vector<glm::vec3> LampPlantPostion2 ={ //one light 
	glm::vec3(262,0,363), 
	glm::vec3(300,0,260) 
}; 
std::vector<float> LampPlant2Orientation={ 
	-90 , 90
};

void generatePointLight(glm::vec3 LampPlantPostion2,int i,glm::vec3 DiffuseColor,glm::vec3 ambient,glm::vec3 specular,float quadratic){

	shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient",glm::value_ptr(ambient)); // Queda como "pointLights[0].light.ambient" 
	shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse",glm::value_ptr(DiffuseColor)); 
	shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular",glm::value_ptr(specular)); 
	shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].position",glm::value_ptr(glm::vec3(LampPlantPostion2))); 
	shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].constant",1.0); 
	shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].linear",0.09); 
	shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].quadratic", quadratic); 
	shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient",glm::value_ptr(ambient)); // Queda como "pointLights[0].light.ambient" 
	shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse",glm::value_ptr(DiffuseColor)); 
	shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular",glm::value_ptr(specular)); 
	shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].position",glm::value_ptr(glm::vec3(LampPlantPostion2))); 
	shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].constant",1.0); 
	shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].linear",0.09); 
	shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].quadratic", quadratic); 
} 

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliHeliBack.loadModel("../models/Helicopter/Mi_24_heli_back.obj");
	modelHeliHeliBack.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Mayow
	mayowModelAnimate.loadModel("../models/mayow/personaje2.fbx");
	mayowModelAnimate.setShader(&shaderMulLighting);
	
	// Cowboy
	cowboyModelAnimate.loadModel("../models/cowboy/Character Running.fbx");
	cowboyModelAnimate.setShader(&shaderMulLighting);

	// Guardian
	guardianModelAnimate.loadModel("../models/boblampclean/boblampclean.md5mesh");
	guardianModelAnimate.setShader(&shaderMulLighting);

	// Cyborg
	cyborgModelAnimate.loadModel("../models/cyborg/cyborg.fbx");
	cyborgModelAnimate.setShader(&shaderMulLighting);

	// Lamps model
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2Post.loadModel("../models/Street_Light/LampPost.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp2Post.setShader(&shaderMulLighting);

    //New lamp 

    modelLampPlant2.loadModel("../models/Lamp2/Tree_Light_2.fbx"); 
	modelLampPlant2.setShader(&shaderMulLighting); 

	// Terreno
	terrain.init();
	terrain.setShader(&shaderTerrain);

	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
		textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureLandingPad.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria

	// Defininiendo texturas del mapa de mezclas
	// Definiendo la textura
	Texture textureR("../Textures/mud.png");
	textureR.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainRID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureR.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, textureR.getWidth(), textureR.getHeight(), 0,
		textureR.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureR.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureR.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureG("../Textures/grassFlowers.png");
	textureG.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainGID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureG.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, textureG.getWidth(), textureG.getHeight(), 0,
		textureG.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureG.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureG.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureB("../Textures/path.png");
	textureB.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureB.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, textureB.getWidth(), textureB.getHeight(), 0,
		textureB.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureB.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureB.freeImage(); // Liberamos memoria

	// Definiendo la textura
	Texture textureBlendMap("../Textures/blendMap.png");
	textureBlendMap.loadImage(); // Cargar la textura
	glGenTextures(1, &textureTerrainBlendMapID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureBlendMap.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, textureBlendMap.getWidth(), textureBlendMap.getHeight(), 0,
		textureBlendMap.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureBlendMap.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureBlendMap.freeImage(); // Liberamos memoria

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliHeliBack.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	mayowModelAnimate.destroy();
	cowboyModelAnimate.destroy();
	guardianModelAnimate.destroy();
	cyborgModelAnimate.destroy();
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLamp2Post.destroy();
    modelLampPlant2.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainBlendMapID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			//fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			//fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			//fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			//fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Controles de mayow
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.02f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	} else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.02f, glm::vec3(0, 1, 0));
		animationMayowIndex = 0;
	}
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 0.02));
		animationMayowIndex = 0;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -0.02));
		animationMayowIndex = 0;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMayow = glm::rotate(modelMatrixMayow, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixCowboy = glm::translate(modelMatrixCowboy, glm::vec3(13.0, 0.05, 0.0));

	modelMatrixGuardian = glm::translate(modelMatrixGuardian, glm::vec3(15, 0.05, 0.0));
	modelMatrixGuardian = glm::rotate(modelMatrixGuardian, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

	modelMatrixCyborg = glm::translate(modelMatrixCyborg, glm::vec3(5.0f, 0.05, 0.0f));


	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.05, 0.05, 0.05)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);

		glm::vec3 spotLightPosition = glm::translate(modelMatrixHeli,glm::vec3(0,0.21,1.689))[3];
		glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.0, 0.2, 1.75, 1.0));

		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("spotLights[0].specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position",
			glm::value_ptr(spotLightPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.direction",
			glm::value_ptr(glm::vec3(0,-1,0)));
		shaderMulLighting.setFloat("spotLights[0].constant",
			1.0);
		shaderMulLighting.setFloat("spotLights[0].linear",
			0.007);
		shaderMulLighting.setFloat("spotLights[0].quadratic",
			0.0003);
		shaderMulLighting.setFloat("spotLights[0].cutOff",
			cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff",
			cos(glm::radians(15.0f)));

		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.1, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.6	, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].position",
			glm::value_ptr(spotLightPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction",
			glm::value_ptr(glm::vec3(0,-1,0)));
		shaderTerrain.setFloat("spotLights[0].constant",
			1.0);
		shaderTerrain.setFloat("spotLights[0].linear",
			0.007);
		shaderTerrain.setFloat("spotLights[0].quadratic",
			0.0003);
		shaderTerrain.setFloat("spotLights[0].cutOff",
			cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff",
			cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount",lamp1Position.size()+lamp2PosOri.size()+LampPlantPostion2.size());
		shaderTerrain.setInt("pointLightCount", lamp1Position.size()+lamp2PosOri.size()+LampPlantPostion2.size());

		for (int i=0;i < lamp1Position.size();i++){
			glm::mat4 modelLampAdjust = glm::mat4(1.0);
			modelLampAdjust = glm::translate(modelLampAdjust,lamp1Position[i]);
			modelLampAdjust = glm::rotate(modelLampAdjust,glm::radians(lamp1Orientation[i]),glm::vec3(0,1,0));
			modelLampAdjust = glm::scale(modelLampAdjust,glm::vec3(0.5f));
			modelLampAdjust = glm::translate(modelLampAdjust,glm::vec3(0,10.35,0));
			glm::vec3 positionLamp = modelLampAdjust[3];
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient", 
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01))); //yellow
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse", 
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular", 
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.position",
				glm::value_ptr(positionLamp));
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].constant",
				1.0);
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].linear",
				0.009);
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].quadratic",
				0.002);
		}

		for (int i=0;i < lamp1Position.size();i++){
			glm::mat4 modelLampAdjust = glm::mat4(1.0);
			modelLampAdjust = glm::translate(modelLampAdjust,lamp1Position[i]);
			modelLampAdjust = glm::rotate(modelLampAdjust,glm::radians(lamp1Orientation[i]),glm::vec3(0,1,0));
			modelLampAdjust = glm::scale(modelLampAdjust,glm::vec3(0.5f));
			modelLampAdjust = glm::translate(modelLampAdjust,glm::vec3(0,10.35,0));
			glm::vec3 positionLamp = modelLampAdjust[3];
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient", 
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01))); //yellow
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse", 
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular", 
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.position",
				glm::value_ptr(positionLamp));
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].constant",
				1.0);
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].linear",
				0.009);
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].quadratic",
				0.002);
		}


		for (int i=lamp1Position.size();i < lamp2PosOri.size() + lamp1Position.size();i++){
			glm::mat4 modelLampAdjust = glm::mat4(1.0);
			modelLampAdjust = glm::translate(modelLampAdjust,lamp2PosOri[i-lamp1Position.size()].first);
			modelLampAdjust = glm::rotate(modelLampAdjust,glm::radians(lamp2PosOri[i-lamp1Position.size()].second),glm::vec3(0,1,0));
			modelLampAdjust = glm::scale(modelLampAdjust,glm::vec3(1.0));
			modelLampAdjust = glm::translate(modelLampAdjust,glm::vec3(0.76,5.0,0));
			glm::vec3 positionLamp = modelLampAdjust[3];
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient", 
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01))); //yellow
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse", 
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular", 
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights["+std::to_string(i)+"].light.position",
				glm::value_ptr(positionLamp));
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].constant",
				1.0);
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].linear",
				0.009);
			shaderMulLighting.setFloat("pointLights["+std::to_string(i)+"].quadratic",
				0.002);
				shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.ambient", 
				glm::value_ptr(glm::vec3(0.2, 0.16, 0.01))); //yellow
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.diffuse", 
				glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.specular", 
				glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights["+std::to_string(i)+"].light.position",
				glm::value_ptr(positionLamp));
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].constant",
				1.0);
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].linear",
				0.009);
			shaderTerrain.setFloat("pointLights["+std::to_string(i)+"].quadratic",
				0.002);
		}

        // NEW MODEL LIGHTS
        for(int i = 0; i < LampPlantPostion2.size();i++){ // Recordar transformacion de abajo haca arribase aplican 
			glm::mat4 matrixAdjustLampPlant2 = glm::mat4(1.0); 
			matrixAdjustLampPlant2 = glm::translate(matrixAdjustLampPlant2,LampPlantPostion2[i]); 
			matrixAdjustLampPlant2 = glm::rotate(matrixAdjustLampPlant2,glm::radians(LampPlant2Orientation[i]),glm::vec3(0,1,0)); 
			matrixAdjustLampPlant2 = glm::scale(matrixAdjustLampPlant2, glm::vec3(1.25)); 
			matrixAdjustLampPlant2= glm::translate(matrixAdjustLampPlant2,glm::vec3(-0.265,2.048,0.82)); 
			glm::vec3 LampPlantPostion2 = glm::vec3(matrixAdjustLampPlant2[3]); 
			generatePointLight(LampPlantPostion2,i,glm::vec3(1.0,1.0,0.0),glm::vec3(0.25,0.25,0.25),glm::vec3(1.0,0.5,0.0),0.08); 
			}
		/*******************************************
		 * Terrain Cesped
		 *******************************************/
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderTerrain.setInt("backgroundTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(80, 80)));
		terrain.setPosition(glm::vec3(100, 0, 100));
		terrain.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelMatrixAircraft[3][1] = terrain.getHeightTerrain(modelMatrixAircraft[3][0], modelMatrixAircraft[3][2]) + 2.0;
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		modelMatrixEclipse[3][1] = terrain.getHeightTerrain(modelMatrixEclipse[3][0], modelMatrixEclipse[3][2]);
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);
		glm::mat4 modelMatrixHeliHeliBack = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeliBack = glm::translate(modelMatrixHeliHeliBack, glm::vec3(0.400524, 2.0928, -5.64124));
		modelMatrixHeliHeliBack = glm::rotate(modelMatrixHeliHeliBack, rotHelHelBack, glm::vec3(1.0, 0.0, 0.0));
		modelMatrixHeliHeliBack = glm::translate(modelMatrixHeliHeliBack, glm::vec3(-0.400524, -2.0928, 5.64124));
		modelHeliHeliBack.render(modelMatrixHeliHeliBack);

		cars_behavoir();

		for (int i=0;i < lamp1Position.size();i++){
			lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x,lamp1Position[1].z);
			modelLamp1.setPosition(lamp1Position[i]);
			modelLamp1.setScale(glm::vec3(0.5));
			modelLamp1.setOrientation(glm::vec3(0,lamp1Orientation[i],0));
			modelLamp1.render();
		}
		for (int i = 0; i < lamp2PosOri.size();i++){
			lamp2PosOri[i].first.y = terrain.getHeightTerrain(lamp2PosOri[i].first.x,
			lamp2PosOri[i].first.z);
			modelLamp2.setPosition(lamp2PosOri[i].first);
			modelLamp2.setOrientation(glm::vec3(0,lamp2PosOri[i].second,0));
			modelLamp2.setScale(glm::vec3(1.0));
			modelLamp2.render();
			modelLamp2Post.setPosition(lamp2PosOri[i].first);
			modelLamp2Post.setOrientation(glm::vec3(0,lamp2PosOri[i].second,0));
			modelLamp2Post.setScale(glm::vec3(1.0));
			modelLamp2Post.render();
		}

        //New plant
        for (int i =0; i < LampPlantPostion2.size();i++){ 
			LampPlantPostion2[i].y = terrain.getHeightTerrain(LampPlantPostion2[i].x,LampPlantPostion2[i].z); 
			modelLampPlant2.setPosition(LampPlantPostion2[i]); 
			modelLampPlant2.setScale(glm::vec3(1.25)); 
			modelLampPlant2.setOrientation(glm::vec3(0,LampPlant2Orientation[i],0)); 
			modelLampPlant2.render(); 
		} 
		/*****************************************
		 * Objetos animados por huesos
		 * **************************************/
		glm::vec3 ejey = glm::normalize(terrain.getNormalTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]));
		glm::vec3 ejex = glm::vec3(modelMatrixMayow[0]);
		glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
		ejex = glm::normalize(glm::cross(ejey, ejez));
		modelMatrixMayow[0] = glm::vec4(ejex, 0.0);
		modelMatrixMayow[1] = glm::vec4(ejey, 0.0);
		modelMatrixMayow[2] = glm::vec4(ejez, 0.0);
		modelMatrixMayow[3][1] = terrain.getHeightTerrain(modelMatrixMayow[3][0], modelMatrixMayow[3][2]);
		glm::mat4 modelMatrixMayowBody = glm::mat4(modelMatrixMayow);
		modelMatrixMayowBody = glm::scale(modelMatrixMayowBody, glm::vec3(0.021f));
		mayowModelAnimate.setAnimationIndex(animationMayowIndex);
		mayowModelAnimate.render(modelMatrixMayowBody);
		animationMayowIndex = 1;

		modelMatrixCowboy[3][1] = terrain.getHeightTerrain(modelMatrixCowboy[3][0], modelMatrixCowboy[3][2]);
		glm::mat4 modelMatrixCowboyBody = glm::mat4(modelMatrixCowboy);
		modelMatrixCowboyBody = glm::scale(modelMatrixCowboyBody, glm::vec3(0.0021f));
		cowboyModelAnimate.render(modelMatrixCowboyBody);

		modelMatrixGuardian[3][1] = terrain.getHeightTerrain(modelMatrixGuardian[3][0], modelMatrixGuardian[3][2]);
		glm::mat4 modelMatrixGuardianBody = glm::mat4(modelMatrixGuardian);
		modelMatrixGuardianBody = glm::scale(modelMatrixGuardianBody, glm::vec3(0.04f));
		guardianModelAnimate.render(modelMatrixGuardianBody);

		modelMatrixCyborg[3][1] = terrain.getHeightTerrain(modelMatrixCyborg[3][0], modelMatrixCyborg[3][2]);
		glm::mat4 modelMatrixCyborgBody = glm::mat4(modelMatrixCyborg);
		modelMatrixCyborgBody = glm::scale(modelMatrixCyborgBody, glm::vec3(0.009f));
		cyborgModelAnimate.setAnimationIndex(1);
		cyborgModelAnimate.render(modelMatrixCyborgBody);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		// Constantes de animaciones
		rotHelHelY += 0.5;
		rotHelHelBack += 0.5;

		glfwSwapBuffers(window);
	}
}

int state = 0;
float advanceCount = 0.0;
float rotCount = 0.0;
float rotWheelsX = 0.0;
float rotWheelsY = 0.0;
int numberAdvance = 0;
int maxAdvance = 0.0;
const float avance = 0.1;
const float giroEclipse = 0.5f;

int state_Lambo = 0;
float advanceCount_Lambo = 0.0;
float rotCount_Lambo = 0.0;
float rotWheelsX_Lambo = 0.0;
float rotWheelsY_Lambo = 0.0;
int numberAdvance_Lambo = 0;
int maxAdvance_Lambo = 0.0;
const float avance_Lambo = 0.1;
const float giro_Lambo = 0.5f;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;
bool Opening_Lambo = false;

void cars_behavoir(){
	// Lambo car
	glDisable(GL_CULL_FACE);
	glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
	modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
	modelLambo.render(modelMatrixLamboChasis);
	glActiveTexture(GL_TEXTURE0);
	glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
	modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
	modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
	modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
	modelLamboLeftDor.render(modelMatrixLamboLeftDor);
	modelLamboRightDor.render(modelMatrixLamboChasis);

	glm::mat4 modelMatrixLamboFrontLeftWheel = glm::mat4(modelMatrixLamboChasis);
	modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel,glm::vec3(1.041,0.374,1.403));
	modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsY_Lambo, glm::vec3(0, 1, 0));
	modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel,rotWheelsX_Lambo,glm::vec3(1,0,0));
	modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel,glm::vec3(-1.041,-0.374,-1.403));	
	
	modelLamboFrontLeftWheel.render(modelMatrixLamboFrontLeftWheel);

	glm::mat4 modelMatrixLamboFrontRightWheel = glm::mat4(modelMatrixLamboChasis);
	modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel,glm::vec3(-1.041,0.374,1.403));
	modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel, rotWheelsY_Lambo, glm::vec3(0, 1, 0));
	modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel,rotWheelsX_Lambo,glm::vec3(1,0,0));
	modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel,glm::vec3(1.041,-0.374,-1.403));
	
	modelLamboFrontRightWheel.render(modelMatrixLamboFrontRightWheel);

	//x1.062	y1.603	0.3975
	glm::mat4 modelMatrixLamboRearLeftWheel = glm::mat4(modelMatrixLamboChasis);
	modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel,glm::vec3(1.062,0.3975,-1.603));
	modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsY_Lambo, glm::vec3(0, 1, 0));
	modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel,rotWheelsX_Lambo,glm::vec3(1,0,0));
	modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel,glm::vec3(-1.062,-0.3975,1.603));

	modelLamboRearLeftWheel.render(modelMatrixLamboRearLeftWheel);

	glm::mat4 modelMatrixLamboRearRightWheel = glm::mat4(modelMatrixLamboChasis);
	modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel,glm::vec3(-1.062,0.3975,-1.603));
	modelMatrixLamboRearRightWheel = glm::rotate(modelMatrixLamboRearRightWheel, rotWheelsY_Lambo, glm::vec3(0, 1, 0));
	modelMatrixLamboRearRightWheel = glm::rotate(modelMatrixLamboRearRightWheel,rotWheelsX_Lambo,glm::vec3(1,0,0));
	modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel,glm::vec3(1.062,-0.3975,1.603));

	modelLamboRearRightWheel.render(modelMatrixLamboRearRightWheel);
	//DoorsLambo();
	// Se regresa el cull faces IMPORTANTE para las puertas
	glEnable(GL_CULL_FACE);

	switch (state_Lambo)
		{
		case 0:
			if(numberAdvance_Lambo == 0)
				maxAdvance_Lambo = 60.0;
			else if(numberAdvance_Lambo == 1)
				maxAdvance_Lambo = 40.0f;
			else if(numberAdvance_Lambo == 2)
				maxAdvance_Lambo = 35.0f;
			else if(numberAdvance_Lambo == 3)
				maxAdvance_Lambo = 40.0f;
			else if(numberAdvance_Lambo == 4)
				maxAdvance_Lambo = 35.0f;
			state_Lambo = 1;
			
			break;
		case 1:
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, avance_Lambo));
			advanceCount_Lambo += avance_Lambo;
			rotWheelsX_Lambo += 0.05;
			rotWheelsY_Lambo -= 0.01;
			if(rotWheelsY_Lambo <= 0)
				rotWheelsY_Lambo = 0;
			if(advanceCount_Lambo > maxAdvance_Lambo){
				advanceCount_Lambo = 0;
				numberAdvance_Lambo++;
				state_Lambo = 2;
			}
			break;
		case 2:
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0, 0, 0.02));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(giro_Lambo),
				glm::vec3(0, 1, 0));
			rotCount_Lambo += giro_Lambo;
			rotWheelsX_Lambo += 0.025;
			rotWheelsY_Lambo += 0.01;
			if(rotWheelsY_Lambo >= 0.5)
				rotWheelsY_Lambo = 0.5;
			if(rotCount_Lambo >= 90.0){
				rotCount_Lambo = 0;
				state_Lambo = 0;
				if(numberAdvance_Lambo==4){
					state_Lambo = 3;
					Opening_Lambo=true;
				}
				if(numberAdvance_Lambo > 4)
					numberAdvance_Lambo = 1;
			}
			break;
			case 3: // tercer estado
			DoorsLambo();
			if(!Opening_Lambo){
				//numberAdvance_Lambo = 1;
				state_Lambo = 0;
			}
			break;
		default:
			break;
		}

	//Eclipse Part


	// Render for the eclipse car
	glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
	modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
	modelEclipseChasis.render(modelMatrixEclipseChasis);

	glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
	modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
	modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
	modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
	modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

	glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
	modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
	modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsY, glm::vec3(0, 1, 0));
	modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
	modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
	modelEclipseRearWheels.render(modelMatrixRearWheels);


	switch (state)
		{
		case 0:
			if(numberAdvance == 0)
				maxAdvance = 65.0;
			else if(numberAdvance == 1)
				maxAdvance = 49.0f;
			else if(numberAdvance == 2)
				maxAdvance = 45.0f;
			else if(numberAdvance == 3)
				maxAdvance = 49.0f;
			else if(numberAdvance == 4)
				maxAdvance = 45.0f;
			state = 1;
			break;
		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance));
			advanceCount += avance;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.01;
			if(rotWheelsY <= 0)
				rotWheelsY = 0;
			if(advanceCount > maxAdvance){
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0, 0, 0.02));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse),
				glm::vec3(0, 1, 0));
			rotCount += giroEclipse;
			rotWheelsX += 0.025;
			rotWheelsY += 0.01;
			if(rotWheelsY >= 0.3)
				rotWheelsY = 0.3;
			if(rotCount >= 90.0){
				rotCount = 0;
				state = 0;
				if(numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		default:
			break;
		}
	

 }
 void DoorsLambo(){
	if(!Opening_Lambo){
		return ;
	}
	switch (stateDoor)
		{
		case 0:
			dorRotCount += 0.5;
			if(dorRotCount > 75)
				stateDoor=1;
			break;
		case 1:
			dorRotCount -= 0.5;
			if(dorRotCount < 0){
				dorRotCount=0;
				stateDoor=2;
			}
			break;
		case 2:
			Opening_Lambo= false;
			stateDoor=0;
		default:
			break;
		}
 }

int main(int argc, char **argv) {
    for (int i=0;i < LampPlantPostion2.size();i++){
		LampPlantPostion2[i] = TransformGIMPCoordenatesToOpenGLPixels(LampPlantPostion2[i]);
	}
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
