////////////////////////////////////////
// cube.h
////////////////////////////////////////

#ifndef _CUBE_H_
#define _CUBE_H_


#include "vector3.h"
#include "matrix4.h"

////////////////////////////////////////////////////////////////////////////////

class SpinningCube {
public:
	SpinningCube();

	
	void Update();
	void Reset();
	void Draw();

	void IncX() { posX += 0.5; }
	void DecX() { posX -= 0.5; }
	void IncY() { posY += 0.5; }
	void DecY() { posY -= 0.5; }
	void IncZ() { posZ += 0.5; }
	void DecZ() { posZ -= 0.5; }

private:
	// Constants
	float Size;
	Vector3 Position;
	Vector3 Axis;

	// Variables
	float Angle;
	Matrix4 WorldMtx;
	Matrix4 RotMtx;
	Matrix4 TransMtx;
	float posX, posY, posZ;
};

////////////////////////////////////////////////////////////////////////////////

/*
SpinningCube is an example of a basic animating object. It can be used as a
pattern for creating more complex objects.
*/

#endif
