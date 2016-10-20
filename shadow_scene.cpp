#include "shadow_scene.h"

Shadow_Scene::Shadow_Scene()
{
	LightProjection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 35.0f);
	//LightProjection = glm::frustum(-1.0f, 1.0f, -1.33f, 1.33f, 1.0f, 20.0f);
	//LightProjection = glm::perspective(glm::radians(60.0f), 1.33f, 0.1f, 1000.0f);
	LightView = glm::lookAt(glm::vec3(-15.0f, 15.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	//LightProjection1 = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 55.0f);
	//LightView1 = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	LightProjection1 = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 35.0f);
	LightView1 = glm::lookAt(glm::vec3(-15.0f, 15.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	Model = glm::mat4(1.0f);
	bumpOn = false;
}

void Shadow_Scene::Update(){

}

void Shadow_Scene::SetDepthMap(GLuint SWIDTH, GLuint SHEIGHT){
	SHADOW_WIDTH = SWIDTH;
	SHADOW_HEIGHT = SHEIGHT;

	supersample = 5;

	//Create FBO
	glGenFramebuffers(1, &depthMapFBO);

	//Creare depth texture
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	//Create space for texture data
	glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT,SHADOW_WIDTH*supersample, SHADOW_HEIGHT*supersample, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//Filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Set wrapping modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	//Attatch depth texture to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	//Disable color rendering
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//LIGHT2
	//Create FBO
	glGenFramebuffers(1, &depthMapFBO1);

	//Creare depth texture
	glGenTextures(1, &depthTexture1);
	glBindTexture(GL_TEXTURE_2D, depthTexture1);
	glActiveTexture(GL_TEXTURE1);


	//Create space for texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH*supersample, SHADOW_HEIGHT*supersample, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//Filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Set wrapping modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);

	//Attatch depth texture to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture1, 0);

	//Disable color rendering
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	unsigned char* tdata;
	int twidth, theight;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	tdata = loadPPM("brick.ppm", twidth, theight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &normalID);
	glBindTexture(GL_TEXTURE_2D, normalID);
	tdata = loadPPM("brick_norm.ppm", twidth, theight);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


}

void Shadow_Scene::DrawForLight() {
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(25.0f, -2.0f, 25.0f);
	glVertex3f(25.0f, -2.0f, -25.0f);
	glVertex3f(-25.0f, -2.0f, -25.0f);
	glVertex3f(-25.0f, -2.0f, 25.0f);
	glEnd();
}

void Shadow_Scene::Draw() {
	lightpass->bind();
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightProjection"), 1, GL_FALSE, glm::value_ptr(LightProjection));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightView"), 1, GL_FALSE, glm::value_ptr(LightView));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Model"), 1, GL_FALSE, glm::value_ptr(Model));

	//Bind FBO and set viewport to size of depth texture
	glViewport(0, 0, SHADOW_WIDTH*supersample, SHADOW_HEIGHT*supersample);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	//Clear
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);

	//Draw scene
	glCullFace(GL_FRONT);
	DrawForLight();
	model->DrawForLight();
	glCullFace(GL_BACK);

	glDisable(GL_POLYGON_OFFSET_FILL);

	//Light2 time

	lightpass->bind();
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightProjection"), 1, GL_FALSE, glm::value_ptr(LightProjection1));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightView"), 1, GL_FALSE, glm::value_ptr(LightView1));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Model"), 1, GL_FALSE, glm::value_ptr(Model));

	//Bind FBO and set viewport to size of depth texture
	glViewport(0, 0, SHADOW_WIDTH*supersample, SHADOW_HEIGHT*supersample);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);

	//Clear
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);

	//Draw scene
	glCullFace(GL_FRONT);
	DrawForLight();
	model->DrawForLight();
	glCullFace(GL_BACK);

	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	shader->bind();

	glUniform1i(glGetUniformLocation(shader->pid, "depthTexture"), 0);
	glUniform1i(glGetUniformLocation(shader->pid, "depthTexture1"), 1);
	glUniform1i(glGetUniformLocation(shader->pid, "modelTexture"), 2);
	glUniform1i(glGetUniformLocation(shader->pid, "normalTexture"), 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalID);

	LightProjection1 = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 35.0f);
	LightView1 = glm::lookAt(glm::vec3(-15.0f, 15.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	glUniform4f(glGetUniformLocation(shader->pid, "matAmbient"), (GLfloat)0.2, (GLfloat)0.3, (GLfloat)0.2, (GLfloat)1.0);
	glUniform4f(glGetUniformLocation(shader->pid, "matDiffuse"), (GLfloat)0.8, (GLfloat)0.7, (GLfloat)0.8, (GLfloat)1.0);
	glUniform4f(glGetUniformLocation(shader->pid, "matSpecular"), (GLfloat)0.2, (GLfloat)0.3, (GLfloat)0.2, (GLfloat)1.0);
	glUniform1f(glGetUniformLocation(shader->pid, "matShininess"), (GLfloat)20);
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightView"), 1, GL_FALSE, glm::value_ptr(LightView));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightProjection"), 1, GL_FALSE, glm::value_ptr(LightProjection));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightView1"), 1, GL_FALSE, glm::value_ptr(LightView1));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "LightProjection1"), 1, GL_FALSE, glm::value_ptr(LightProjection1));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Projection"), 1, GL_FALSE, glm::value_ptr(Cam->getProjection()));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "View"), 1, GL_FALSE, glm::value_ptr(Cam->getView()));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Model"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniform1i(glGetUniformLocation(shader->pid, "bumpOn"), bumpOn);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(25.0f, -2.0f, 25.0f);
	glVertex3f(25.0f, -2.0f, -25.0f);
	glVertex3f(-25.0f, -2.0f, -25.0f);
	glVertex3f(-25.0f, -2.0f, 25.0f);
	glEnd();
	//model->DrawShadow();
}

unsigned char* Shadow_Scene::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}