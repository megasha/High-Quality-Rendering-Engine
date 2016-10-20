////////////////////////////////////////
// camera.h
////////////////////////////////////////

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4.h"
#include "shader.h"
#include <glm/mat4x4.hpp>

////////////////////////////////////////////////////////////////////////////////

class Camera {
public:
	Camera();

	void setCamera(Vector3 centerE, Vector3 lookP, Vector3 up);
	glm::mat4 getModelView();
	glm::mat4 getView();
	glm::mat4 getProjection();

	void Update();
	void Reset();
	void Draw();

	// Access functions
	void SetAspect(float a)		{ Aspect = a; }
	void SetDistance(float d)	{ Distance = d; }
	void SetAzimuth(float a)	{ Azimuth = a; }
	void SetIncline(float i)	{ Incline = i; }

	float GetDistance()			{ return Distance; }
	float GetAzimuth()			{ return Azimuth; }
	float GetIncline()			{ return Incline; }
	void SetShader(Shader *s) { shader = s; }

	void IncX() { camX += 0.5; }
	void DecX() { camX -= 0.5; }
	void IncY() { camY += 0.5; }
	void DecY() { camY -= 0.5; }

	float camX, camY;

private:
	Shader *shader;
	Matrix4 cameraC;
	// Perspective controls
	float FOV;		// Field of View Angle
	float Aspect;	// Aspect Ratio
	float NearClip;	// Near clipping plane distance
	float FarClip;	// Far clipping plane distance
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 ModelView;

	// Polar controls
	float Distance;	// Distance of the camera eye position to the origin
	float Azimuth;	// Rotation of the camera eye position around the Y axis
	float Incline;	// Angle of the camera eye position over the XZ plane
};

////////////////////////////////////////////////////////////////////////////////

/*
The Camera class provides a simple means to controlling the 3D camera. It could
be extended to support more interactive controls. Ultimately. the camera sets the
GL projection and viewing matrices.
*/

#endif
