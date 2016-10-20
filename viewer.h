////////////////////////////////////////
// viewer.h
///////////////////////////////////////

#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "camera.h"
#include "cube.h"
#include "model.h"
#include "cubemap.h"
#include "shadow_scene.h"
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

namespace Globals {
	extern Cubemap cubemap;
	extern Model model;
	extern Camera Cam;
	extern Shadow_Scene sScene;
	extern std::vector<Shader*> shaderList;
};

//////////////////////////////////////////////////////////////////////////////

class Viewer {
public:
	Viewer(int argc, char **argv);
	~Viewer();


	void Update();
	void StateController();
	void Reset();
	void Draw();

	//Display FPS in Console
	void fpsUpdate();

	void Quit();


	//Event handlers
	void Resize(int x, int y);
	void Keyboard(int key, int x, int y);
	void SpecialKeyboard(int key, int x, int y);
	void MouseButton(int btn, int state, int x, int y);
	void MouseMotion(int x, int y);
	std::vector<GLfloat> light0;
	std::vector<GLfloat> light1;

private:
	SpinningCube Cube;
	//Cubemap cubemap;
	int WinX, WinY;
	int WindowHandle;
	int state;
	int maxState;
	int frame, timeFrame, timebase;
	bool didIrrad;

	// Input
	bool LeftDown, MiddleDown, RightDown;
	int MouseX, MouseY;
};

//////////////////////////////////////////////////////////////////////////////

#endif