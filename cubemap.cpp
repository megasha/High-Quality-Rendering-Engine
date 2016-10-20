#include "Cubemap.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


Cubemap::Cubemap()
{
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* Cubemap::loadPPM(const char* filename, int& width, int& height)
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

// load image file into texture object
void Cubemap::loadTexture()
{
	float points[] = {
		-50.0f, 50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		50.0f, -50.0f, -50.0f,
		50.0f, -50.0f, -50.0f,
		50.0f, 50.0f, -50.0f,
		-50.0f, 50.0f, -50.0f,

		-50.0f, -50.0f, 50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f, 50.0f, -50.0f,
		-50.0f, 50.0f, -50.0f,
		-50.0f, 50.0f, 50.0f,
		-50.0f, -50.0f, 50.0f,

		50.0f, -50.0f, -50.0f,
		50.0f, -50.0f, 50.0f,
		50.0f, 50.0f, 50.0f,
		50.0f, 50.0f, 50.0f,
		50.0f, 50.0f, -50.0f,
		50.0f, -50.0f, -50.0f,

		-50.0f, -50.0f, 50.0f,
		-50.0f, 50.0f, 50.0f,
		50.0f, 50.0f, 50.0f,
		50.0f, 50.0f, 50.0f,
		50.0f, -50.0f, 50.0f,
		-50.0f, -50.0f, 50.0f,

		-50.0f, 50.0f, -50.0f,
		50.0f, 50.0f, -50.0f,
		50.0f, 50.0f, 50.0f,
		50.0f, 50.0f, 50.0f,
		-50.0f, 50.0f, 50.0f,
		-50.0f, 50.0f, -50.0f,

		-50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f, 50.0f,
		50.0f, -50.0f, -50.0f,
		50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f, 50.0f,
		50.0f, -50.0f, 50.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	int twidth, theight;   // texture width/height [pixels]
	unsigned char* tdata;  // texture pixel data

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Load image file
	for (GLuint i = 0; i < 6; i++) {
		switch (i) {
		case 0:
			tdata = loadPPM("posx.ppm", twidth, theight); break;
		case 1:
			tdata = loadPPM("negx.ppm", twidth, theight); break;
		case 2:
			tdata = loadPPM("posy.ppm", twidth, theight); break;
		case 3:
			tdata = loadPPM("negy.ppm", twidth, theight); break;
		case 4:
			tdata = loadPPM("posz.ppm", twidth, theight); break;
		case 5:
			tdata = loadPPM("negz.ppm", twidth, theight); break;
		default:
			fprintf(stderr, "LOADTEXTURE: COULDN'T READ FILE");
		}

		if (tdata == NULL) return;

		// Generate the texture
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);

	}

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TEXTUREWIDTH = twidth;
	TEXTUREHEIGHT = theight;

}

void Cubemap::CreateIrradiance() {

	glGenTextures(1, &IrradianceTextureMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceTextureMap);

	//std::vector<GLubyte> testData(TEXTUREWIDTH * TEXTUREHEIGHT * 256, 128);


	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, TEXTUREWIDTH, TEXTUREHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &IrradianceFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, IrradianceFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, IrradianceTextureMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAME BUFFER ERROR!" << std::endl;
		return;
	}

	//Save viewport
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	//Prepare framebuffer and texture
	glBindFramebuffer(GL_FRAMEBUFFER, IrradianceFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, IrradianceTextureMap, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, IrradianceTextureMap, 0);
	glViewport(0, 0, TEXTUREWIDTH, TEXTUREHEIGHT);

	//Render to cubemap
	irradianceMapShader->bind();

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "skybox"), 0);
	glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "width"), TEXTUREWIDTH);
	glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "height"), TEXTUREHEIGHT);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glm::mat4 irradProj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f);
	glm::mat4 irradView;
	for (int i = 0; i < 6; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceTextureMap, 0);
		
		switch (i) {
			//Right
			case 0: irradView = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 1.0, 0.0, 0.0);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 0);
					break;
			//Left
			case 1: irradView = glm::lookAt(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 0.0, 1.0, 0.0);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 1);
					break;
			//Top
			case 2: irradView = glm::lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 0.0, 0.0, 1.0);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 2);
					break;
			//Bottom
			case 3: irradView = glm::lookAt(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 0.2, 0.2, 0.2);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 3);
					break;
			//Back
			case 4: irradView = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 0.0, 0.0, 0.0);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 4);
					break;
			//Front
			case 5: irradView = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					glUniform3f(glGetUniformLocation(irradianceMapShader->pid, "colorTest"), 1.0, 1.0, 1.0);
					glUniform1i(glGetUniformLocation(irradianceMapShader->pid, "currFace"), 5);
					break;
			default:
				fprintf(stderr, "ERROR! ERROR! ERROR!\n");
		}

		glUniformMatrix4fv(glGetUniformLocation(irradianceMapShader->pid, "Projection"), 1, GL_FALSE, glm::value_ptr(irradProj));
		glUniformMatrix4fv(glGetUniformLocation(irradianceMapShader->pid, "View"), 1, GL_FALSE, glm::value_ptr(irradView));

		
		DrawCube();
		//model->Draw();

	}

	//Reset framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_viewport[2], m_viewport[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Cubemap::DrawCube() {
	glBegin(GL_QUADS);
	// Draw front face:
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);

	// Draw left side:
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, 1.0);

	// Draw right side:
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(1.0, -1.0, -1.0);

	// Draw back face:
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(-1.0, -1.0, -1.0);

	// Draw top side:
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(-1.0, 1.0, -1.0);

	// Draw bottom side:
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(1.0, -1.0, -1.0);
	glVertex3f(1.0, -1.0, 1.0);
	glVertex3f(-1.0, -1.0, 1.0);
	glEnd();
}

void Cubemap::Draw() {
	shader->bind();

	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Projection"), 1, GL_FALSE, glm::value_ptr(Cam->getProjection()));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "View"), 1, GL_FALSE, glm::value_ptr(Cam->getView()));

	glDepthMask(GL_FALSE);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->pid, "skybox"), 0);
	glUniform1i(glGetUniformLocation(modelShader->pid, "skybox"), 0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceTextureMap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}