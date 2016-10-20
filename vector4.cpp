#include "vector4.h"

Vector4 Vector4::operator+(const Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator-(const Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}


//We use this if v is a point
void Vector4::Set(Vector3& v) {
	x = v[0];
	y = v[1];
	z = v[2];
	w = 1;

}

Vector4 Vector4::Dehomogenize()
{
	float nx, ny, nz, nw;
	nx = x / w;
	ny = y / w;
	nz = z / w;
	nw = w / w;
	return Vector4(nx, ny, nz, nw);
}

float Vector4::TransposeMult(Vector4& v){
	float ret = 0;
	ret += x*v[0];
	ret += y*v[1];
	ret += z*v[2];
	ret += w*v[3];
	return ret;
}
