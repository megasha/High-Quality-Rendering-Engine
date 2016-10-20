#ifndef _SHADOW_SCENE_H_
#define _SHADOW_SCENE_H_

#include "camera.h"
#include "shader.h"
#include "model.h"
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shadow_Scene {
public:
	Shadow_Scene();
	void Update();
	void Draw();
	void DrawForLight();
	bool bumpOn;
	unsigned char* loadPPM(const char*, int&, int&);
	void SetLightPass(Shader* s) { lightpass = s; }
	void SetShader(Shader* s) { shader = s; }
	void SetCamera(Camera *c) { Cam = c; }
	void SetModel(Model *m) { model = m; }
	void SetBumpOff() { bumpOn = false; }
	void SetBumpOn() { bumpOn = true; }
	void SetDepthMap(GLuint SHADOW_WIDTH, GLuint SHADOW_HEIGHT);
	Shader *shader;
	Shader *lightpass;
	glm::mat4 LightProjection1;
	glm::mat4 LightView1;
private:
	GLuint supersample;
	GLuint SHADOW_WIDTH;
	GLuint SHADOW_HEIGHT;
	GLuint depthTexture;
	GLuint depthMapFBO;
	GLuint depthTexture1;
	GLuint depthMapFBO1;
	GLuint textureID;
	GLuint normalID;
	Camera *Cam;
	Model *model;
	glm::mat4 Model;
	glm::mat4 LightProjection;
	glm::mat4 LightView;

	std::vector<GLfloat> quad;
};


#endif