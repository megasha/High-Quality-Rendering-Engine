////////////////////////////////////////
// cube.cpp
////////////////////////////////////////

#include "cube.h"
#include <GL/glut.h>
#include "vector3.h"

////////////////////////////////////////////////////////////////////////////////

SpinningCube::SpinningCube() {
	// Initialize constant data
	WorldMtx.Identity();
	TransMtx.Identity();
	RotMtx.Identity();
	Size = 1.0f;
	Position = Vector3(0.0f, 0.0f, 0.0f);
	Axis = Vector3(0.0f, 1.0f, 0.0f);
	posX = 0;
	posY = 0;
	posZ = 0;

	// Resets variable data
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Update() {
	// Update (animate) any variable properties
	Angle += 1.0f;
	RotMtx.makeRotate(Angle, Axis);
    TransMtx.makeTranslate(posX,posY,posZ);
    WorldMtx = TransMtx*RotMtx;
	WorldMtx.Transpose();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Reset() {
	// Reset dynamic variables to a default state
	Angle = 0.0f;
	WorldMtx.Identity();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Draw() {
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(WorldMtx.getPointer());
	glBegin(GL_QUADS);

	glEnd;

	glutWireCube(Size);
}

////////////////////////////////////////////////////////////////////////////////
