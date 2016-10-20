////////////////////////////////////////
// camera.cpp
////////////////////////////////////////

#include "vector3.h"
#include "camera.h"
#include <iostream>
#include <GL/glut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"



////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	cameraC = Matrix4();
	Reset();
}

void Camera::setCamera(Vector3 centerE, Vector3 lookP, Vector3 up) {
	//Compute Z-axis
	Vector3 zAxis = centerE - lookP;
	zAxis.Normalize();

	//Compute X-axis
	Vector3 xAxis;
	xAxis.Cross(up, zAxis);
	xAxis.Normalize();

	//Computer Y-axis
	Vector3 yAxis;
	yAxis.Cross(zAxis, xAxis);

	cameraC.setCameraMatrix3Vect(xAxis, yAxis, zAxis, Vector3(0, 0, 0));
	cameraC.transpose();

	Matrix4 trans;
	trans.makeTranslate(-centerE[0], -centerE[1], -centerE[2]);

	cameraC = cameraC * trans;
	cameraC.transpose();

	//cameraC.print("");
}

//Tester method
glm::mat4 Camera::getModelView()
{
	return ModelView;
}

glm::mat4 Camera::getView()
{
	return View;
}

glm::mat4 Camera::getProjection()
{
	return Projection;
}



////////////////////////////////////////////////////////////////////////////////

void Camera::Update() {

	
	Projection = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);
	View = glm::mat4(1.0);
	View = glm::translate(View, glm::vec3(camX, camY, -Distance));
	View = glm::rotate(View, glm::radians(Incline), glm::vec3(1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, glm::radians(Azimuth), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelView = Projection * View;
	

	//light1
	//Projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 35.0f);
	//View = glm::lookAt(glm::vec3(-15.0f, 15.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	//Light2
	//Projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 10.0f, 55.0f);
	//View = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f,1.0f,0.0f));
	

	/* FOR ORTHO TESTING
	Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f);
	View = glm::lookAt(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	*/
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	FOV = 60.0f;
	Aspect = 1.33f;
	NearClip = 0.1f;
	FarClip = 1000.0f;

	Distance = 5.0f;
	Azimuth = 0.0f;
	Incline = 0.0f;

	camX = 0;
	camY = 0;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Draw() {

}

////////////////////////////////////////////////////////////////////////////////
