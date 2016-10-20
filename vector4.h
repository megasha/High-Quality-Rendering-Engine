#ifndef _VECTOR_4_
#define _VECTOR_4_

#include <iostream>
#include "vector3.h"

class Vector4
{
public:
	Vector4() :
		x(0.0f), y(0.0f), z(0.0f), w(0.0f)			{}

	Vector4(float x0, float y0, float z0, float w0) :
		x(x0), y(y0), z(z0), w(w0)					{}
	void Set(float x0, float y0, float z0, float w0)	{ x = x0; y = y0; z = z0; w = w0; }
	void Set(Vector3& v);
	float &operator[](int i)				{ return(((float*)this)[i]); }

	void Print(char *name = 0) const		{ if (name) printf("%s=", name); printf("{%f,%f,%f,%f}\n", x, y, z, w); }

	Vector4 operator+(const Vector4&);
	Vector4 operator-(const Vector4&);
	float TransposeMult(Vector4&);
	Vector4 Dehomogenize();


public:
	float x, y, z, w;
};

#endif

