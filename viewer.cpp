///////////////////////////////////////
//viewer.cpp
///////////////////////////////////////

#include "viewer.h"
#include <algorithm>

#include <iostream>
#include <iomanip>
#include <fstream>

#include <glm/vec3.hpp>
#include "shader.h"

#define WINDOWTITLE "CSE190 PA3"
#define MODEL "models/teapot.off"

std::string mesh_file_s;

//////////////////////////////////////////////////////////////////////////////

static Viewer *VIEWER;

namespace Globals
{
	Cubemap cubemap;
	Model model;
	Camera Cam;
	Shadow_Scene sScene;
	std::vector<Shader*> shaderList;
};

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	VIEWER = new Viewer(argc, argv);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	Shader *toon = new Shader("toon_shading.vert", "toon_shading.frag");
	Shader *toon1Light = new Shader("toon_shading1.vert", "toon_shading1.frag");
	Shader *phong = new Shader("phong_shading.vert", "phong_shading.frag");
	//Shader *test = new Shader("test.vert", "test.frag");
	Shader *modelEnv = new Shader("env_model.vert", "env_model.frag");
	Shader *shadow = new Shader("shadow_scene.vert","shadow_scene.frag");
	Shader *irradianceMap = new Shader("irradiance_map.vert", "irradiance_map.frag");
	Shader *lightpass = new Shader("lightpass.vert", "lightpass.frag");
	Shader *environment = new Shader("environment.vert", "environment.frag");
	Shader *cook = new Shader("cook.vert", "cook.frag");
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	//Send lightposition to shaders
	phong->bind();
	glUniform3f(glGetUniformLocation(phong->pid, "lightPos0"), VIEWER->light0[0], VIEWER->light0[1], VIEWER->light0[2]);
	glUniform3f(glGetUniformLocation(phong->pid, "lightPos1"), VIEWER->light1[0], VIEWER->light1[1], VIEWER->light1[2]);
	cook->bind();
	glUniform3f(glGetUniformLocation(cook->pid, "lightPos0"), VIEWER->light0[0], VIEWER->light0[1], VIEWER->light0[2]);
	glUniform3f(glGetUniformLocation(cook->pid, "lightPos1"), VIEWER->light1[0], VIEWER->light1[1], VIEWER->light1[2]);
	toon->bind();
	glUniform3f(glGetUniformLocation(toon->pid, "lightPos0"), VIEWER->light0[0], VIEWER->light0[1], VIEWER->light0[2]);
	glUniform3f(glGetUniformLocation(toon->pid, "lightPos1"), VIEWER->light1[0], VIEWER->light1[1], VIEWER->light1[2]);
	toon1Light->bind();
	glUniform3f(glGetUniformLocation(toon->pid, "lightPos0"), VIEWER->light0[0], VIEWER->light0[1], VIEWER->light0[2]);


	Globals::cubemap.SetShader(environment);
	Globals::cubemap.SetModelShader(modelEnv);
	Globals::cubemap.SetIrradianceMapShader(irradianceMap);
	Globals::model.SetShader(toon); //MODEL HERE
	Globals::model.SetLightPass(lightpass);
	Globals::sScene.SetShader(shadow);
	Globals::sScene.SetLightPass(lightpass);
	Globals::model.SetShadowPass(shadow);

	Globals::cubemap.loadTexture();
	Globals::sScene.SetDepthMap(640, 480);
	Globals::cubemap.CreateIrradiance();


	Globals::shaderList.push_back(toon);
	Globals::shaderList.push_back(phong);
	Globals::shaderList.push_back(modelEnv);
	Globals::shaderList.push_back(shadow);
	Globals::shaderList.push_back(environment);
	Globals::shaderList.push_back(lightpass);
	Globals::shaderList.push_back(toon1Light);
	Globals::shaderList.push_back(cook);

	Globals::shaderList.push_back(irradianceMap);





	glutMainLoop();	
	return 0;
}

//////////////////////////////////////////////////////////////////////////////

static void display()									{ VIEWER->Draw(); }
static void idle()										{ VIEWER->Update(); }
static void resize(int x, int y)						{ VIEWER->Resize(x, y); }
static void specialKeyboard(int key, int x, int y) { VIEWER->SpecialKeyboard(key, x, y); }
static void keyboard(unsigned char key, int x, int y)	{ VIEWER->Keyboard(key, x, y); }
static void mousebutton(int btn, int state, int x, int y)	{ VIEWER->MouseButton(btn, state, x, y); }
static void mousemotion(int x, int y)						{ VIEWER->MouseMotion(x, y); }


Viewer::Viewer(int argc, char**argv) {
	state = 0;
	maxState = 6;
	WinX = 640;
	WinY = 480;
	LeftDown = MiddleDown = RightDown = false;
	timeFrame = timebase = frame = 0;
	MouseX = MouseY = 0;

	//Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(WinX, WinY);
	glutInitWindowPosition(0, 0);
	WindowHandle = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);
	glutSetWindow(WindowHandle);

	glEnable(GL_TEXTURE_2D);   // enable texture mapping
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);   // enables depth testing
	glDepthFunc(GL_LEQUAL);    // configure depth testing
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // really nice perspective calculations
	//glEnable(GL_COLOR_MATERIAL);

	


	// Background Color
	glClearColor(0.4, 0.4, 0.4, 1.);
	//glClearColor(0., 0., 0., 1.);

	//Set lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	GLfloat light_position0[] = { -15.0, 15.0, -2.0, 0.0 };
	GLfloat light_ambient0[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse0[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	
	
	GLfloat light_position1[] = { 10, 10, 10, 0.0 };
	GLfloat light_ambient1[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse1[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	
	

	light0 = std::vector<GLfloat>(light_position0, light_position0 + sizeof(light_position0) / sizeof(GLfloat));
	light1 = std::vector<GLfloat>(light_position1, light_position1 + sizeof(light_position1) / sizeof(GLfloat));



	//Callbacks
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
	glutPassiveMotionFunc(mousemotion);
	glutReshapeFunc(resize);

	//Initialize components here
	Globals::Cam.SetAspect(float(WinX) / float(WinY));


	//Camera setup for objects
	Globals::model.SetCamera(&Globals::Cam);
	Globals::cubemap.SetCamera(&Globals::Cam);
	Globals::sScene.SetCamera(&Globals::Cam);

	Globals::sScene.SetModel(&Globals::model);
	Globals::cubemap.SetModel(&Globals::model);
	
	//Model load
	Globals::model.Load(MODEL);
	//cubemap.loadTexture();
	didIrrad = false;
}

//////////////////////////////////////////////////////////////////////////////

Viewer::~Viewer() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::fpsUpdate()
{
	frame++;
	timeFrame = glutGet(GLUT_ELAPSED_TIME);
	if (timeFrame - timebase > 1000)
	{
		float f = frame*1000.0 / (timeFrame - timebase);
		std::setprecision(6);
		std::cout << "FPS: " << f << std::endl;
		timebase = timeFrame;
		frame = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Update() {
	//Update components in the world
	Globals::Cam.Update();
	//Globals::sScene.Update();
	//Cube.Update();
	Globals::model.Update();
	//fpsUpdate();

	//Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Reset() {
	Globals::Cam.Reset();
	Globals::model.Reset();
	Globals::Cam.SetAspect(float(WinX) / float(WinY));
	//Cube.Reset();


}

void Viewer::StateController() {
	switch (state) {
	case 0:
		Globals::model.SetShader(Globals::shaderList[0]);
		break;
	case 1:
		Globals::model.SetShader(Globals::shaderList[1]);
		break;
	case 2:
		Globals::model.SetShader(Globals::shaderList[7]);
		break;
	case 3:
		Globals::model.SetShader(Globals::shaderList[2]);
		break;
	
	case 4:
		Globals::model.SetShader(Globals::shaderList[6]);
		Globals::sScene.SetBumpOff();
		break;
	case 5:
		Globals::model.SetShader(Globals::shaderList[6]);
		Globals::sScene.SetBumpOn();
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Draw() {
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	StateController();
	Globals::Cam.Draw();

	if(state == 3) Globals::cubemap.Draw();
	if(state == 4 || state==5) Globals::sScene.Draw();
	Globals::model.Draw();
	
	//Cube.Draw();

	glFinish();
	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Resize(int x, int y) {
	WinX = x;
	WinY = y;

	//Camera stuff
	Globals::Cam.SetAspect(float(WinX) / float(WinY));
	Globals::sScene.SetDepthMap(WinX, WinY);


}

//////////////////////////////////////////////////////////////////////////////

void Viewer::Keyboard(int key, int x, int y) {
	//std::cout << key << std::endl;
	switch (key) {
	case 0x1b:		// Escape
		Quit();
		break;
	case 'r':
		Reset();
		break;
	case 'O':
		Globals::Cam.IncX();
		break;
	case 'o':
		Globals::Cam.DecX();
		break;
	case 'P':
		Globals::Cam.IncY();
		break;
	case 'p':
		Globals::Cam.DecY();
		break;

	case 'd':
		Globals::model.IncX();
		break;
	case 'a':
		Globals::model.DecX();
		break;
	case 'e':
		Globals::model.IncY();
		break;
	case 'q':
		Globals::model.DecY();
		break;
	case 's':
		Globals::model.IncZ();
		break;
	case 'w':
		Globals::model.DecZ();
		break;
	case 'z':
		Globals::model.IncRotX();
		break;
	case 'Z':
		Globals::model.DecRotX();
		break;
	case 'x':
		Globals::model.IncRotY();
		break;
	case 'X':
		Globals::model.DecRotY();
		break;
	case 'c':
		Globals::model.IncRotZ();
		break;
	case 'C':
		Globals::model.DecRotZ();
		break;

	case 'v':
		Globals::model.ToggleSpin();
		break;
	}
}

void Viewer::SpecialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		--state;
		if (state == -1) state = maxState - 1; 
		//std::cout << "STATE: " << state << std::endl;
		break;
	case GLUT_KEY_RIGHT:
		state = (state + 1) % maxState;
		//std::cout << "STATE: " << state << std::endl;
		break;
	}

	StateController();
}

////////////////////////////////////////////////////////////////////////////////

void Viewer::MouseButton(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON) {
		LeftDown = (state == GLUT_DOWN);
	}
	else if (btn == GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state == GLUT_DOWN);
	}
	else if (btn == GLUT_RIGHT_BUTTON) {
		RightDown = (state == GLUT_DOWN);
	}
}

//////////////////////////////////////////////////////////////////////////////

void Viewer::MouseMotion(int nx, int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Globals::Cam.SetAzimuth(Globals::Cam.GetAzimuth() + dx*rate);
		Globals::Cam.SetIncline(Globals::Cam.GetIncline() - dy*rate);
	}
	if (RightDown) {
		const float rate = 0.01f;
		Globals::Cam.SetDistance(Globals::Cam.GetDistance()*(1.0f - dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
