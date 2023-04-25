/*
	Simple example of 3D with Blit3D
*/
//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"

#include "Mesh.h"
#include "SceneManager.h"
#include "SceneNodes.h"
#include "APC.h"
#include "Joystick.h"
#include "Config.h"

Blit3D* blit3D = NULL;

//GLOBAL DATA
GLSLProgram* shader = NULL;

glm::mat4 modelMatrix;
std::atomic<float> angle = 0;

Configuration configuration;
Joystick joystick;
SceneManager* sceneManager = NULL;
APC * apc = NULL;

bool stripped = false;

glm::vec3 initialTankPos(0.f, 0.f, -20.f);

void Init()
{
	blit3D->SetMode(Blit3DRenderMode::BLIT3D);

	sceneManager = new SceneManager();

	shader = new GLSLProgram(); // instantiate the new shader
	shader->compileShaderFromFile("vertex.glsl", GLSLShader::GLSLShaderType::VERTEX);
	shader->compileShaderFromFile("fragment.glsl", GLSLShader::GLSLShaderType::FRAGMENT);
	//bind attributes for the shader layout
	shader->bindAttribLocation(0, "position");
	shader->bindAttribLocation(1, "normal");
	shader->bindAttribLocation(2, "texUV");
	shader->link();
	// link the new shader
	shader->use();
	// tell blit to use the new shader
	blit3D->SetMode(Blit3DRenderMode::BLIT3D, shader);
	// set blit mode to 3D using new shader
	//blit3D->viewMatrix = glm::mat4();

	//3d perspective projection
	blit3D->projectionMatrix = glm::perspective(45.0f, (GLfloat)(blit3D->screenWidth) / (GLfloat)(blit3D->screenHeight), 0.1f, 1000.0f);
	shader->setUniform("projectionMatrix", blit3D->projectionMatrix);
	shader->setUniform("viewMatrix", blit3D->viewMatrix);

	glEnable(GL_BLEND);// enable blending	
	glEnable(GL_DEPTH_TEST);// enables proper 3D depth drawing	
	glEnable(GL_CULL_FACE);// backs of triangles are no longer drawn	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// more blending

	apc = new APC(sceneManager->sceneGraph, initialTankPos, shader, 5.f, 20.f, 2.5f);//Make an instance of the tank

	//Load config file for joystick values
	if (!LoadConfig(configuration))
	{
		std::cout << "ERROR reading the config file!\n\n";
	}
	else
		DisplayConfig(configuration);

	//joystick stuff
	joystick.Init();
}

void DeInit(void)
{
	//Delete stuff from memory
	if (sceneManager != NULL) delete sceneManager;
	if (shader != NULL) delete shader;
	if (apc != NULL) delete apc;
}

void Update(double seconds)
{
	//cameraOffset.z += moveZ * moveSpeed *seconds;
	if (seconds > 0.15) seconds = 0.15;

	sceneManager->UpdateWorld((float)seconds);

	joystick.DoJoystick(); //handle joystick polling
	//process joystick values
	joystick.ProcessJoystickAxis(joystick.leftJoystickX, configuration);
	joystick.ProcessJoystickAxis(joystick.leftJoystickY, configuration);
	joystick.ProcessJoystickAxis(joystick.rightJoystickX, configuration);
	joystick.ProcessJoystickAxis(joystick.rightJoystickY, configuration);
	joystick.ProcessJoystickAxis(joystick.leftBumper, configuration);
	joystick.ProcessJoystickAxis(joystick.rightBumper, configuration);

	//Update the tank with joystick input
	apc->Update((float)seconds, joystick.rightBumper, joystick.leftBumper, joystick.leftJoystickX, joystick.rightJoystickX, joystick.rightJoystickY);
}

void Draw(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a 	
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager->DrawWorld();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		apc->ResetPosition(initialTankPos);//Reset the tank to its original position 
	}
}

int main(int argc, char* argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::DECORATEDWINDOW, 1024, 1024);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}