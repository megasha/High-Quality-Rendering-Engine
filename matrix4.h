////////////////////////////////////////
// matrix4.h
////////////////////////////////////////

#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "vector3.h"
#include "vector4.h"

#define M_PI 3.14159265358979323846


////////////////////////////////////////////////////////////////////////////////

class Matrix4 {
public:
	Matrix4();
	Matrix4(float ax, float bx, float cx, float dx,
		float ay, float by, float cy, float dy,
		float az, float bz, float cz, float dz,
		float aw, float bw, float cw, float dw);

	void Print(const char *s = 0) const;
	Matrix4& operator=(const Matrix4&);
	float* getPointer()								{ return (float*)this; }
	void Identity();
	void Transpose();

	void Clear();
	Matrix4 operator*(const Matrix4&);
	void operator+=(const Matrix4&);
	Matrix4 operator+(const Matrix4&);
	Vector4 operator*(Vector4&);
	void makeRotateX(float);
	void makeRotateY(float);
	void makeRotateZ(float);
	void makeRotate(float, Vector3&);
	void makeScale(float, float, float);
	void makeTranslate(float, float, float);
	void makeQuadric(float, float, float, float);
	void transpose();
	void makeTranslate(Vector3&);
	void print(std::string);
	Vector3 getObjCoordinates();
	void setCameraMatrix3Vect(Vector3&, Vector3&, Vector3&, Vector3&);
	bool IsDetZero();
	Vector3 GetOptimalVert();
	bool Inverse();

public:
	float m[4][4];
};

class Matrix34 {
public:
	Matrix34() { Identity(); pad0 = pad1 = pad2 = 0.0f; pad3 = 1.0f; }
	Matrix34(float ax, float bx, float cx, float dx,
		float ay, float by, float cy, float dy,
		float az, float bz, float cz, float dz);
	Matrix34(Matrix4&);
	void Set(Matrix4&);
	Vector3 Dot(const Matrix34 &n, Vector3& m);
	void Identity();
	bool Inverse();

	static Matrix34 IDENTITY;

public:
	Vector3 a; float pad0;
	Vector3 b; float pad1;
	Vector3 c; float pad2;
	Vector3 d; float pad3;
};

#endif
