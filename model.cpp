#include "model.h"

#include "token.h"
#include <stdlib.h>

#include <GL/glut.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_FORCE_DEGREES




Model::Model()
{
	/*
	OffsetMtx.Identity();
	WorldMtx.Identity();
	TransMtx.Identity();
	*/
	spin = true;
	posX = 0;
	posY = 0;
	posZ = 0;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
}

void Model::Update() {
	if (spin) {
		rotY += 0.2;
		if(rotY > 360) rotY = 0.0;
	}
	RotMtxX = glm::rotate(glm::mat4(1.0f), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	RotMtxY = glm::rotate(glm::mat4(1.0f), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	RotMtxZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	RotMtxX = RotMtxZ * RotMtxY * RotMtxX;
	float xdist = maxX - minX;
	float ydist = maxY - minY;
	xdist *= ydist;
	if (xdist < 21.0f) xdist = 21.0f;
	float ratio = 21.0f / xdist;
	RotMtxX = glm::scale(RotMtxX, glm::vec3(ratio, ratio, ratio));
	OffsetMtx = glm::translate(glm::mat4(1.0f), glm::vec3(-midX*ratio, -midY*ratio, -midZ*ratio));
	TransMtx = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
	TransMtx = TransMtx * OffsetMtx;
	ModelMtx = TransMtx * RotMtxX;
	
}

void Model::Reset() {
	spin = true;
	posX = 0;
	posY = 0;
	posZ = 0;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
}



bool Model::Load(const char* file) {
	Tokenizer token;
	Vector3 *faceTemp;

	if ((file == NULL) || (token.Open(file) == 0))	{
		//Add default file here
		return false;
	}

	token.FindToken("OFF");
	vertices = token.GetInt();
	vertAlt = vertices;
	faces = token.GetInt();
	token.SkipLine();


	//Set Face Adjacency Table
	vertFaceAdj.resize(vertices);

	float x, y, z;
	for (int i = 0; i < vertices; i++)
	{
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();	
		vertArray.push_back(new Vector3(x, y, z));

		if (i == 0) {
			minX = x;
			minY = y;
			minZ = z;

			maxX = x;
			maxY = y;
			maxZ = z;
		}

		else {
			if (x < minX)
				minX = x;
			else if (x > maxX)
				maxX = x;

			if (y < minY)
				minY = y;
			else if (y > maxY)
				maxY = y;

			if (z < minZ)
				minZ = z;
			else if (z > maxZ)
				maxZ = z;
		}
	}

	midX = (minX + maxX) / 2.0;
	midY = (minY + maxY) / 2.0;
	midZ = (minZ + maxZ) / 2.0;

	for (int i = 0; i < faces; i++)
	{
		token.GetInt();
		x = token.GetInt();
		y = token.GetInt();
		z = token.GetInt();

		faceTemp = new Vector3(x, y, z);
		faceArray.push_back(faceTemp);

		vertFaceAdj[(int)x].push_back(faceTemp);
		vertFaceAdj[(int)y].push_back(faceTemp);
		vertFaceAdj[(int)z].push_back(faceTemp);
	}

	MakeVertexNormal();

	token.Close();
	return true;
}


void Model::DrawShadow() {
	int tempIndex;
	Vector3 *normA, *normB, *normC;
	Vector3 *vecA, *vecB, *vecC;

	glUniformMatrix4fv(glGetUniformLocation(shadowpass->pid, "Projection"), 1, GL_FALSE, glm::value_ptr(Cam->getProjection()));
	glUniformMatrix4fv(glGetUniformLocation(shadowpass->pid, "View"), 1, GL_FALSE, glm::value_ptr(Cam->getView()));
	glUniformMatrix4fv(glGetUniformLocation(shadowpass->pid, "Model"), 1, GL_FALSE, glm::value_ptr(ModelMtx));
	//glUniform4f(glGetUniformLocation(shadowpass->pid, "color"), (GLfloat)0.1, (GLfloat)0.5, (GLfloat) 0.8, (GLfloat)1.0);

	//glLoadMatrixf(glm::value_ptr(Cam->getCam()));

	for (int i = 0; i < faceArray.size(); i++)
	{

		tempIndex = (int)((*faceArray[i])[0]);
		vecA = vertArray[tempIndex];
		normA = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[1]);
		vecB = vertArray[tempIndex];
		normB = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[2]);
		vecC = vertArray[tempIndex];
		normC = vertNormArray[tempIndex];

		glBegin(GL_TRIANGLES);
		
		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_amb_diff[] = { 0.1, 0.5, 0.8, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 10.0);


		glNormal3f((*normA)[0], (*normA)[1], (*normA)[2]);
		glVertex3f((*vecA)[0], (*vecA)[1], (*vecA)[2]);
		glNormal3f((*normB)[0], (*normB)[1], (*normB)[2]);
		glVertex3f((*vecB)[0], (*vecB)[1], (*vecB)[2]);
		glNormal3f((*normC)[0], (*normC)[1], (*normC)[2]);
		glVertex3f((*vecC)[0], (*vecC)[1], (*vecC)[2]);
		glEnd();
	}
}

void Model::Draw() {
	
	int tempIndex;
	Vector3 *normA, *normB, *normC;
	Vector3 *vecA, *vecB, *vecC;

	shader->bind();

	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Projection"), 1, GL_FALSE, glm::value_ptr(Cam->getProjection()));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "View"), 1, GL_FALSE, glm::value_ptr(Cam->getView()));
	glUniformMatrix4fv(glGetUniformLocation(shader->pid, "Model"), 1, GL_FALSE, glm::value_ptr(ModelMtx));

	//glLoadMatrixf(glm::value_ptr(Cam->getCam()));

	for (int i = 0; i < faceArray.size(); i++)
	{

		tempIndex = (int)((*faceArray[i])[0]);
		vecA = vertArray[tempIndex];
		normA = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[1]);
		vecB = vertArray[tempIndex];
		normB = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[2]);
		vecC = vertArray[tempIndex];
		normC = vertNormArray[tempIndex];

		glBegin(GL_TRIANGLES);

		GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat mat_amb_diff[] = { 0.1, 0.5, 0.8, 1.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

		glNormal3f((*normA)[0], (*normA)[1], (*normA)[2]);
		glVertex3f((*vecA)[0], (*vecA)[1], (*vecA)[2]);
		glNormal3f((*normB)[0], (*normB)[1], (*normB)[2]);
		glVertex3f((*vecB)[0], (*vecB)[1], (*vecB)[2]);
		glNormal3f((*normC)[0], (*normC)[1], (*normC)[2]);
		glVertex3f((*vecC)[0], (*vecC)[1], (*vecC)[2]);
		glEnd();
	}
	
	


}

void Model::DrawForLight() {
	int tempIndex;
	Vector3 *normA, *normB, *normC;
	Vector3 *vecA, *vecB, *vecC;

	lightpass->bind();
	glUniformMatrix4fv(glGetUniformLocation(lightpass->pid, "Model"), 1, GL_FALSE, glm::value_ptr(ModelMtx));

	for (int i = 0; i < faceArray.size(); i++)
	{

		tempIndex = (int)((*faceArray[i])[0]);
		vecA = vertArray[tempIndex];
		normA = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[1]);
		vecB = vertArray[tempIndex];
		normB = vertNormArray[tempIndex];

		tempIndex = (int)((*faceArray[i])[2]);
		vecC = vertArray[tempIndex];
		normC = vertNormArray[tempIndex];

		glBegin(GL_TRIANGLES);

		glNormal3f((*normA)[0], (*normA)[1], (*normA)[2]);
		glVertex3f((*vecA)[0], (*vecA)[1], (*vecA)[2]);
		glNormal3f((*normB)[0], (*normB)[1], (*normB)[2]);
		glVertex3f((*vecB)[0], (*vecB)[1], (*vecB)[2]);
		glNormal3f((*normC)[0], (*normC)[1], (*normC)[2]);
		glVertex3f((*vecC)[0], (*vecC)[1], (*vecC)[2]);
		glEnd();
	}
}

void Model::MakeVertexNormal() {
	for (int i = 0; i < vertFaceAdj.size(); i++) {
		Vector3 *currFace;
		Vector3 currFaceNorm;
		Vector3 *accum = new Vector3();
		for (int j = 0; j < vertFaceAdj[i].size(); j++) {
			currFace = vertFaceAdj[i][j];
			currFaceNorm = MakeFaceNormal(*currFace);
			*accum += currFaceNorm;
		}
		accum->Normalize();
		vertNormArray.push_back(accum);
	}
}

Vector3 Model::MakeFaceNormal(Vector3& a) {
	Vector3 *vecA;
	Vector3 *vecB;
	Vector3 *vecC;

	Vector3 ret;

	vecA = vertArray[a[0]];
	vecB = vertArray[a[1]];
	vecC = vertArray[a[2]];

	Vector3 tVec0 = *vecB - *vecA;
	Vector3 tVec1 = *vecC - *vecA;

	ret.Cross(tVec0, tVec1);
	ret.Normalize();

	return ret;
}