////////////////////////////////////////
// vector3.h
///////////////////////////////////////

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <iostream>

//////////////////////////////////////////////////////////////////////////////

class Vector3
{
public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f)	{ dirty = false; }
	Vector3(float x0, float y0, float z0) :
		x(x0), y(y0), z(z0)					{
		dirty = false;
	}
	void Set(float x0, float y0, float z0)	{ x = x0; y = y0; z = z0; }
	void Zero()								{ x = y = z = 0.0f; }
	void Print(char *name = 0) const		{ if (name) printf("%s=", name); printf("{%f,%f,%f}\n", x, y, z); }
	float &operator[](int i)				{ return(((float*)this)[i]); }

	Vector3 operator+ (const Vector3& v);
	Vector3 operator+= (const Vector3& v);
	Vector3 operator- (const Vector3& v);
	Vector3 operator-= (const Vector3& v);
	Vector3 operator* (float a);
	Vector3 operator*= (float a);
	Vector3 operator* (const Vector3& v);
	Vector3 operator*= (const Vector3& v);
	Vector3 operator/ (float a);
	Vector3 operator/= (float a);
	Vector3 operator/ (const Vector3& v);
	Vector3 operator/= (const Vector3& v);
	Vector3 Negate();
	Vector3 Scale(float);
	float at(int);
	void SetDirty() { dirty = true; }
	void UndoDirty() { dirty = false; }
	void Lerp(float t, const Vector3 a, const Vector3 b);
	bool  GetDirty() { return dirty; }
	float Dot(const Vector3&);
	void Cross(const Vector3&, const Vector3&);
	float Mag2();
	float Mag();
	Vector3 Normalize();

public:
	float x, y, z;
	bool dirty;
};

//////////////////////////////////////////////////////////////////////////////

#endif