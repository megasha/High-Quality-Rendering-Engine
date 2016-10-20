#ifndef _MODEL_H_
#define _MODEL_H_

#include "vector3.h"
#include "matrix4.h"
#include <vector>
#include "shader.h"
#include "camera.h"
#include <glm/mat4x4.hpp>


class Model
{
public:
	Model();
	bool Load(const char* file);
	void Update();
	void Draw();
	void Reset();
	void DrawShadow();
	void DrawForLight();
	void MakeVertexNormal();
	Vector3 MakeFaceNormal(Vector3&);
	void SetShader(Shader* s) { shader = s; }
	void SetLightProjection(glm::mat4 m) { LightProjection = m; }
	void SetLightView(glm::mat4 m) { LightView = m; }
	void SetLightPass(Shader* s) { lightpass = s; }
	void SetShadowPass(Shader* s) { shadowpass = s; }
	Shader * shader;
	Shader *lightpass;
	Shader *shadowpass;

	void IncX() { posX += 0.5; }
	void DecX() { posX -= 0.5; }
	void IncY() { posY += 0.5; }
	void DecY() { posY -= 0.5; }
	void IncZ() { posZ += 0.5; }
	void DecZ() { posZ -= 0.5; }
	void IncRotX() { rotX += 5; }
	void IncRotY() { rotY += 5; }
	void IncRotZ() { rotZ += 5; }
	void DecRotX() { rotX -= 5; }
	void DecRotY() { rotY -= 5; }
	void DecRotZ() { rotZ -= 5; }
	void ToggleSpin() { spin = !spin; }

	void SetCamera(Camera *c) { Cam = c; }

private:
	Camera *Cam;
	int vertices, faces, vertAlt;
	std::vector<Vector3*>  vertArray; //Only info aboout vert location
	std::vector<Vector3*>  faceArray; //Info about vertices that make it up
	std::vector<Vector3*>  vertNormArray;
	std::vector< std::vector <Vector3*> > vertFaceAdj;

	float minX, minY, maxX, maxY, minZ, maxZ;
	float midX, midY, midZ;

	/*
	Matrix4 OffsetMtx;
	Matrix4 WorldMtx;
	Matrix4 RotMtxX;
	Matrix4 RotMtxY;
	Matrix4 RotMtxZ;
	Matrix4 TransMtx;
	*/

	glm::mat4 OffsetMtx;
	glm::mat4 ModelMtx;
	glm::mat4 RotMtxX;
	glm::mat4 RotMtxY;
	glm::mat4 RotMtxZ;
	glm::mat4 TransMtx;
	glm::mat4 LightProjection;
	glm::mat4 LightView;

	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	bool spin;
};

#endif

