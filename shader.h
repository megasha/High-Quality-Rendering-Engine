/*
This program is a modified version of So Tamaoka's shader loader/manager under the
GNU General Public License. The code has been adapted to use glew rather than glee.
*/

#ifndef _SHADER_H_
#define _SHADER_H_

#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>


class Shader
{

public:
	GLhandleARB pid;

public:
	Shader(const char *vert, const char *frag, bool isFile = true);
	~Shader();

	void printLog(const char* tag = "");

	void bind();
	void unbind();

private:
	char* read(const char *filename);
	void setup(const char *vs, const char *fs);
};

#endif
