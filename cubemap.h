#ifndef _CUBEMAP_H_
#define _CUBEMAP_H_

#include <stdlib.h>
#include <stdio.h>

#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <GL/glew.h>
#include <GL/glut.h>
#include "shader.h"
#include <vector>
#include "camera.h"
#include "model.h"

class Cubemap {
public:
	Cubemap();
	unsigned char* loadPPM(const char*, int&, int&);
	void loadTexture();
	void CreateIrradiance();
	void Draw();
	void SetShader(Shader *s) { shader = s; }
	void SetIrradianceMapShader(Shader *s) { irradianceMapShader = s; }
	void SetModelShader(Shader *s) { modelShader = s; }
	void SetCamera(Camera *c) { Cam = c; }
	void SetModel(Model *m) { model = m; }

	Model *model;
	Camera *Cam;
	Shader *shader;
	Shader *modelShader;
	Shader *irradianceMapShader;
	GLuint textureID;
	GLuint vbo, vao;
	GLuint texture[6];
	int TEXTUREWIDTH, TEXTUREHEIGHT;
	GLuint IrradianceFBO;
	GLuint IrradianceTextureMap;
	void DrawCube();
};
#endif

