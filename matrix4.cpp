////////////////////////////////////////
// matrix4.cpp
////////////////////////////////////////

#include "matrix4.h"

Matrix4::Matrix4()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}
}

Matrix4::Matrix4(float ax, float bx, float cx, float dx,
				 float ay, float by, float cy, float dy,
				 float az, float bz, float cz, float dz,
				 float aw, float bw, float cw, float dw) 
{
	m[0][0] = ax; m[0][1] = bx; m[0][2] = cx; m[0][3] = dx;
	m[1][0] = ay; m[1][1] = by; m[1][2] = cy; m[1][3] = dy;
	m[2][0] = az; m[2][1] = bz; m[2][2] = cz; m[2][3] = dz;
	m[3][0] = aw; m[3][1] = bw; m[3][2] = cw; m[3][3] = dw;
}

void Matrix4::Clear()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			m[i][j] = 0;
		}
	}
}

void Matrix4::transpose()
{
	Matrix4 temp;
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			temp.m[j][i] = m[i][j];
		}
	}
	*this = temp;  // copy temporary values to this matrix
}

Matrix4& Matrix4::operator=(const Matrix4& m2)
{
	if (this != &m2)
	{
		for (int i = 0; i<4; ++i)
		{
			for (int j = 0; j<4; ++j)
			{
				m[i][j] = m2.m[i][j];
			}
		}
	}
	return *this;
}

// set matrix to identity matrix
void Matrix4::Identity()
{
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			if (i == j)
				m[i][j] = 1.0f;
			else
				m[i][j] = 0.0f;
		}
	}
}

// transpose the matrix (mirror at diagonal)
void Matrix4::Transpose()
{
	Matrix4 temp;
	for (int i = 0; i<4; ++i)
	{
		for (int j = 0; j<4; ++j)
		{
			temp.m[j][i] = m[i][j];
		}
	}
	*this = temp;  // copy temporary values to this matrix
}

Matrix4 Matrix4::operator*(const Matrix4& m2)
{
	Matrix4 temp;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			for (int k = 0; k<4; k++)
			{
				temp.m[i][j] += m[i][k] * m2.m[k][j];
			}
		}
	}
	return temp;
}


void Matrix4::operator+=(const Matrix4& m2){
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = m[i][j] + m2.m[i][j];
		}
	}
}

Matrix4 Matrix4::operator+(const Matrix4& m2){
	Matrix4 temp;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] = m[i][j] + m2.m[i][j];
		}
	}
	return temp;
}

Vector4 Matrix4::operator*(Vector4& v)
{
	Vector4 temp(0, 0, 0, 0);
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			temp[i] += m[i][j] * v[j];
		}
	}
	return temp;
}

void Matrix4::makeRotateX(float angle)
{
	angle = angle / 180 * M_PI;
	Identity();
	m[1][1] = cos(angle);
	m[1][2] = -sin(angle);
	m[2][1] = sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateY(float angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	Identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateZ(float angle)
{
	angle = angle / 180 * M_PI;
	Identity();
	m[0][0] = cos(angle);
	m[0][1] = -sin(angle);
	m[1][0] = sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4::setCameraMatrix3Vect(Vector3& xAxis,Vector3& yAxis,Vector3& zAxis,Vector3& eColumn)
{
	Identity();
	m[0][0] = xAxis[0];
	m[1][0] = xAxis[1];
	m[2][0] = xAxis[2];

	m[0][1] = yAxis[0];
	m[1][1] = yAxis[1];
	m[2][1] = yAxis[2];

	m[0][2] = zAxis[0];
	m[1][2] = zAxis[1];
	m[2][2] = zAxis[2];

	m[0][3] = eColumn[0];
	m[1][3] = eColumn[1];
	m[2][3] = eColumn[2];
}

void Matrix4::makeRotate(float angle, Vector3& axis)
{
	//Must pass in a a unit vector
	angle = angle / 180 * M_PI;
	Identity();
	m[0][0] = 1 + ((1 - cos(angle)) * ((axis[0] * axis[0]) - 1));
	m[0][1] = (-axis[2] * sin(angle)) + ((1 - cos(angle)) * axis[0] * axis[1]);
	m[0][2] = (axis[1] * sin(angle)) + ((1 - cos(angle)) * axis[0] * axis[2]);
	m[1][0] = (axis[2] * sin(angle)) + ((1 - cos(angle)) * axis[1] * axis[0]);
	m[1][1] = 1 + ((1 - cos(angle)) * ((axis[1] * axis[1]) - 1));
	m[1][2] = (-axis[0] * sin(angle)) + ((1 - cos(angle)) * axis[1] * axis[2]);
	m[2][0] = (-axis[1] * sin(angle)) + ((1 - cos(angle)) * axis[2] * axis[0]);
	m[2][1] = (axis[0] * sin(angle)) + ((1 - cos(angle)) * axis[2] * axis[1]);
	m[2][2] = 1 + ((1 - cos(angle)) * ((axis[2] * axis[2]) - 1));
}

void Matrix4::makeScale(float sx, float sy, float sz)
{
	Identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4::makeTranslate(float tx, float ty, float tz)
{
	Identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}

void Matrix4::makeQuadric(float a, float b, float c, float d) {
	Identity();
	m[0][0] = a * a;
	m[0][1] = a * b;
	m[0][2] = a * c;
	m[0][3] = a * d;
	m[1][0] = a * b;
	m[1][1] = b * b;
	m[1][2] = b * c;
	m[1][3] = b * d;
	m[2][0] = a * c;
	m[2][1] = b * c;
	m[2][2] = c * c;
	m[2][3] = c * d;
	m[3][0] = a * d;
	m[3][1] = b * d;
	m[3][2] = c * d;
	m[3][3] = d * d;
}

void Matrix4::Print(const char *s) const {
	Vector4 a(m[0][0], m[0][1], m[0][2], m[0][3]);
	Vector4 b(m[1][0], m[1][1], m[1][2], m[1][3]);
	Vector4 c(m[2][0], m[2][1], m[2][2], m[2][3]);
	Vector4 d(m[3][0], m[3][1], m[3][2], m[3][3]);
	if (s) printf("%s={\n", s);
	else printf("matrix={\n");
	a.Print();
	b.Print();
	c.Print();
	d.Print();
	printf("}\n");
}

bool Matrix4::Inverse() {
	float det = (m[0][0] * m[1][1] * m[2][2] * m[3][3]) + (m[0][0] * m[1][2] * m[2][3] * m[3][1]) + (m[0][0] * m[1][3] * m[2][1] * m[3][2]) 
		      + (m[0][1] * m[1][0] * m[2][3] * m[3][2]) + (m[0][1] * m[1][2] * m[2][0] * m[3][3]) + (m[0][1] * m[1][3] * m[2][2] * m[3][0])
			  + (m[0][2] * m[1][0] * m[2][1] * m[3][3]) + (m[0][2] * m[1][1] * m[2][3] * m[3][0]) + (m[0][2] * m[1][3] * m[2][0] * m[3][1])
			  + (m[0][3] * m[1][0] * m[2][2] * m[3][1]) + (m[0][3] * m[1][1] * m[2][0] * m[3][2]) + (m[0][3] * m[1][2] * m[2][1] * m[3][0])
			  - (m[0][0] * m[1][1] * m[2][3] * m[3][2]) - (m[0][0] * m[1][2] * m[2][1] * m[3][3]) - (m[0][0] * m[1][3] * m[2][2] * m[3][1])
			  - (m[0][1] * m[1][0] * m[2][2] * m[3][3]) - (m[0][1] * m[1][2] * m[2][3] * m[3][0]) - (m[0][1] * m[1][3] * m[2][0] * m[3][2])
			  - (m[0][2] * m[1][0] * m[2][3] * m[3][1]) - (m[0][2] * m[1][1] * m[2][0] * m[3][3]) - (m[0][2] * m[1][3] * m[2][1] * m[3][0])
			  - (m[0][3] * m[1][0] * m[2][1] * m[3][2]) - (m[0][3] * m[1][1] * m[2][2] * m[3][0]) - (m[0][3] * m[1][2] * m[2][0] * m[3][1]);

	if (det <= 0.1 && det >= -0.1) return false;

	float b00 = m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] 
		      - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] - m[1][3] * m[2][2] * m[3][1];
	float b01 = m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] + m[0][3] * m[2][2] * m[3][1] 
		      - m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2];
	float b02 = m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] 
		      - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] - m[0][3] * m[1][2] * m[3][1];
	float b03 = m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1] 
		      - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2];

	float b10 = m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] + m[1][3] * m[2][2] * m[3][0] 
		      - m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2];
	float b11 = m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] 
		      - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] - m[0][3] * m[2][2] * m[3][0];
	float b12 = m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] + m[0][3] * m[1][2] * m[3][0] 
		      - m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2];
	float b13 = m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2]
		      - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0];

	float b20 = m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] 
		      - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] - m[1][3] * m[2][1] * m[3][0];
	float b21 = m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] + m[0][3] * m[2][1] * m[3][0] 
		      - m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1];
	float b22 = m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] 
		      - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] - m[0][3] * m[1][1] * m[3][0];
	float b23 = m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0] 
		      - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1];

	float b30 = m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] + m[1][2] * m[2][1] * m[3][0]
		      - m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1];
	float b31 = m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] 
		      - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] - m[0][2] * m[2][1] * m[3][0];
	float b32 = m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] + m[0][2] * m[1][1] * m[3][0]
		      - m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1];
	float b33 = m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
		      - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0];

	m[0][0] = b00;
	m[0][1] = b01;
	m[0][2] = b02;
	m[0][3] = b03;
	m[1][0] = b10;
	m[1][1] = b11;
	m[1][2] = b12;
	m[1][3] = b13;
	m[2][0] = b20;
	m[2][1] = b21;
	m[2][2] = b22;
	m[2][3] = b23;
	m[3][0] = b30;
	m[3][1] = b31;
	m[3][2] = b32;
	m[3][3] = b33;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] /= det;

	return true;


}

bool Matrix4::IsDetZero() {
	float det = (m[0][0] * m[1][1] * m[2][2] * m[3][3]) + (m[0][0] * m[1][2] * m[2][3] * m[3][1]) + (m[0][0] * m[1][3] * m[2][1] * m[3][2])
		+ (m[0][1] * m[1][0] * m[2][3] * m[3][2]) + (m[0][1] * m[1][2] * m[2][0] * m[3][3]) + (m[0][1] * m[1][3] * m[2][2] * m[3][0])
		+ (m[0][2] * m[1][0] * m[2][1] * m[3][3]) + (m[0][2] * m[1][1] * m[2][3] * m[3][0]) + (m[0][2] * m[1][3] * m[2][0] * m[3][1])
		+ (m[0][3] * m[1][0] * m[2][2] * m[3][1]) + (m[0][3] * m[1][1] * m[2][0] * m[3][2]) + (m[0][3] * m[1][2] * m[2][1] * m[3][0])
		- (m[0][0] * m[1][1] * m[2][3] * m[3][2]) - (m[0][0] * m[1][2] * m[2][1] * m[3][3]) - (m[0][0] * m[1][3] * m[2][2] * m[3][1])
		- (m[0][1] * m[1][0] * m[2][2] * m[3][3]) - (m[0][1] * m[1][2] * m[2][3] * m[3][0]) - (m[0][1] * m[1][3] * m[2][0] * m[3][2])
		- (m[0][2] * m[1][0] * m[2][3] * m[3][1]) - (m[0][2] * m[1][1] * m[2][0] * m[3][3]) - (m[0][2] * m[1][3] * m[2][1] * m[3][0])
		- (m[0][3] * m[1][0] * m[2][1] * m[3][2]) - (m[0][3] * m[1][1] * m[2][2] * m[3][0]) - (m[0][3] * m[1][2] * m[2][0] * m[3][1]);

	if (det <= 0.1 && det >= -0.1) return true;
	return false;
}

Vector3 Matrix4::GetOptimalVert() {
	Vector3 ret;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
	float det = (m[0][0] * m[1][1] * m[2][2] * m[3][3]) + (m[0][0] * m[1][2] * m[2][3] * m[3][1]) + (m[0][0] * m[1][3] * m[2][1] * m[3][2])
		      + (m[0][1] * m[1][0] * m[2][3] * m[3][2]) + (m[0][1] * m[1][2] * m[2][0] * m[3][3]) + (m[0][1] * m[1][3] * m[2][2] * m[3][0])
		      + (m[0][2] * m[1][0] * m[2][1] * m[3][3]) + (m[0][2] * m[1][1] * m[2][3] * m[3][0]) + (m[0][2] * m[1][3] * m[2][0] * m[3][1])
		      + (m[0][3] * m[1][0] * m[2][2] * m[3][1]) + (m[0][3] * m[1][1] * m[2][0] * m[3][2]) + (m[0][3] * m[1][2] * m[2][1] * m[3][0])
		      - (m[0][0] * m[1][1] * m[2][3] * m[3][2]) - (m[0][0] * m[1][2] * m[2][1] * m[3][3]) - (m[0][0] * m[1][3] * m[2][2] * m[3][1])
		      - (m[0][1] * m[1][0] * m[2][2] * m[3][3]) - (m[0][1] * m[1][2] * m[2][3] * m[3][0]) - (m[0][1] * m[1][3] * m[2][0] * m[3][2])
		      - (m[0][2] * m[1][0] * m[2][3] * m[3][1]) - (m[0][2] * m[1][1] * m[2][0] * m[3][3]) - (m[0][2] * m[1][3] * m[2][1] * m[3][0])
		      - (m[0][3] * m[1][0] * m[2][1] * m[3][2]) - (m[0][3] * m[1][1] * m[2][2] * m[3][0]) - (m[0][3] * m[1][2] * m[2][0] * m[3][1]);
	
	float b03 = m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1]
		      - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2];
	float b13 = m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2]
		      - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0];
	float b23 = m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0]
	       	  - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1];
	float b33 = m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
		      - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0];

	b03 /= det;
	b13 /= det;
	b23 /= det;

	ret.Set(b03, b13, b23);
	return ret;
	
}

Matrix34 Matrix34::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f);

Matrix34::Matrix34(float ax, float bx, float cx, float dx,
	float ay, float by, float cy, float dy,
	float az, float bz, float cz, float dz) {
	a.Set(ax, ay, az);
	b.Set(bx, by, bz);
	c.Set(cx, cy, cz);
	d.Set(dx, dy, dz);

	pad0 = pad1 = pad2 = 0.0f;
	pad3 = 1.0f;
}

Matrix34::Matrix34(Matrix4& mat){
	a.Set(mat.m[0][0], mat.m[1][0], mat.m[2][0]);
	b.Set(mat.m[0][1], mat.m[1][1], mat.m[2][1]);
	c.Set(mat.m[0][2], mat.m[1][2], mat.m[2][2]);
	d.Set(mat.m[0][3], mat.m[1][3], mat.m[2][3]);

	pad0 = pad1 = pad2 = 0.0f;
	pad3 = 1.0f;
}

void Matrix34::Set(Matrix4& mat){
	a.Set(mat.m[0][0], mat.m[1][0], mat.m[2][0]);
	b.Set(mat.m[0][1], mat.m[1][1], mat.m[2][1]);
	c.Set(mat.m[0][2], mat.m[1][2], mat.m[2][2]);
	d.Set(mat.m[0][3], mat.m[1][3], mat.m[2][3]);

	pad0 = pad1 = pad2 = 0.0f;
	pad3 = 1.0f;
}

void Matrix34::Identity() {
	a.Set(1.0f, 0.0f, 0.0f);
	b.Set(0.0f, 1.0f, 0.0f);
	c.Set(0.0f, 0.0f, 1.0f);
	d.Set(0.0f, 0.0f, 0.0f);
}

Vector3 Matrix34::Dot(const Matrix34 &n, Vector3& m) {
	float ax = (n.a.x*m.at(0)) + (n.b.x*m.at(1)) + (n.c.x*m.at(2)) + (n.d.x);
	float ay = (n.a.y*m.at(0)) + (n.b.y*m.at(1)) + (n.c.y*m.at(2)) + (n.d.y);
	float az = (n.a.z*m.at(0)) + (n.b.z*m.at(1)) + (n.c.z*m.at(2)) + (n.d.z);

	Vector3 temp(ax, ay, az);
	return temp;
}

inline float Det22(float ax, float ay, float bx, float by) { return ax*by - ay*bx; }


bool Matrix34::Inverse() {
	float subx, suby, subz;
	subx = Det22(b.y, b.z, c.y, c.z);
	suby = Det22(b.x, b.z, c.x, c.z);
	subz = Det22(b.x, b.y, c.x, c.y);

	float Det = a.x*subx - a.y*suby + a.z*subz; // Determinant
	//if (Det == 0.0f) {
	if (Det <= 0.05f && Det >= -0.05f) {
		printf("ERROR: Matrix34::Inverse()- Matrix not invertible\n");
		Identity();
		return false;
	}
	Det = 1.0f / Det;		// Inverse of the determinant

	Matrix34 Temp;
	Temp.a.x = subx*Det;
	Temp.b.x = -suby*Det;
	Temp.c.x = subz*Det;
	Temp.d.x = -(d.x*Temp.a.x + d.y*Temp.b.x + d.z*Temp.c.x);

	subx = Det22(a.y, a.z, c.y, c.z);
	suby = Det22(a.x, a.z, c.x, c.z);
	subz = Det22(a.x, a.y, c.x, c.y);

	Temp.a.y = -subx*Det;
	Temp.b.y = suby*Det;
	Temp.c.y = -subz*Det;
	Temp.d.y = -(d.x*Temp.a.y + d.y*Temp.b.y + d.z*Temp.c.y);

	subx = Det22(a.y, a.z, b.y, b.z);
	suby = Det22(a.x, a.z, b.x, b.z);
	subz = Det22(a.x, a.y, b.x, b.y);

	Temp.a.z = subx*Det;
	Temp.b.z = -suby*Det;
	Temp.c.z = subz*Det;
	Temp.d.z = -(d.x*Temp.a.z + d.y*Temp.b.z + d.z*Temp.c.z);

	(*this) = Temp;
	return true;
}