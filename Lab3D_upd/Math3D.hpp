#pragma once
#include <cmath>
#include <vector>

class Vector3D
{
public:
	float x, y, z, w;
	Vector3D() : x(0), y(0), z(0), w(1) {}
	Vector3D(float xx, float yy, float zz) : x(xx), y(yy), z(zz), w(1) {}

	friend Vector3D operator + (Vector3D v1, Vector3D v2) { return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
	friend Vector3D operator - (Vector3D v1, Vector3D v2) { return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
	friend Vector3D operator * (Vector3D v, float f) { return Vector3D(v.x * f, v.y * f, v.z * f); }
	friend Vector3D operator / (Vector3D v, float f) { return Vector3D(v.x / f, v.y / f, v.z / f); }
	friend Vector3D operator - (Vector3D v) { return v * -1; }

	friend float dot_prod(Vector3D v1, Vector3D v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	friend Vector3D cross_prod(Vector3D a, Vector3D b) { return Vector3D(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
	void norm() { float l = sqrt(x * x + y * y + z * z); x /= l; y /= l; z /= l; }
};

class Matrix4x4
{
	float m[4][4] = { 0 };
public:
	Matrix4x4() {}
	Matrix4x4(std::vector<std::vector<float>> v) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] = v[i][j];
	}

	static Matrix4x4 get_proj_mat(const int width, const int height, const float zNear, const float zFar, const float FOV) {
		const float PI = 3.14159f;
		return Matrix4x4(std::vector<std::vector<float>> {
			{ ((float)height / (float)width) / tanf(FOV / 2 / 180 * PI), 0, 0, 0 },
			{ 0, 1 / tanf(FOV / 2 / 180 * PI), 0, 0 },
			{ 0, 0, zFar / (zFar - zNear), 1 },
			{ 0, 0, -zFar * zNear / (zFar - zNear), 0 }
		});
	}

	static Matrix4x4 get_translate(float dx, float dy, float dz) {
		Matrix4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = dx;
		matrix.m[3][1] = dy;
		matrix.m[3][2] = dz;
		return matrix;
	}

	static Matrix4x4 get_scale(float sx, float sy, float sz) {
		Matrix4x4 matrix;
		matrix.m[0][0] = sx;
		matrix.m[1][1] = sy;
		matrix.m[2][2] = sz;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static Matrix4x4 get_rot_x(float angle)
	{
		Matrix4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(angle);
		matrix.m[1][2] = sinf(angle);
		matrix.m[2][1] = -sinf(angle);
		matrix.m[2][2] = cosf(angle);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
	static Matrix4x4 get_rot_y(float angle)
	{
		Matrix4x4 matrix;
		matrix.m[0][0] = cosf(angle);
		matrix.m[0][2] = sinf(angle);
		matrix.m[2][0] = -sinf(angle);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(angle);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}
	static Matrix4x4 get_rot_z(float angle)
	{
		Matrix4x4 matrix;
		matrix.m[0][0] = cosf(angle);
		matrix.m[0][1] = sinf(angle);
		matrix.m[1][0] = -sinf(angle);
		matrix.m[1][1] = cosf(angle);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	static Matrix4x4 get_point_at_mat(Vector3D& pos, Vector3D& target, Vector3D up)
	{
		Vector3D newForward = target - pos;
		newForward.norm();

		Vector3D newUp = up - newForward * dot_prod(up, newForward);
		newUp.norm();

		Vector3D newRight = cross_prod(newUp, newForward);

		Matrix4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;
	}

	void invert()
	{
		Matrix4x4 matrix;
		matrix.m[0][0] = m[0][0]; matrix.m[0][1] = m[1][0]; matrix.m[0][2] = m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m[0][1]; matrix.m[1][1] = m[1][1]; matrix.m[1][2] = m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m[0][2]; matrix.m[2][1] = m[1][2]; matrix.m[2][2] = m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m[3][0] * matrix.m[0][0] + m[3][1] * matrix.m[1][0] + m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m[3][0] * matrix.m[0][1] + m[3][1] * matrix.m[1][1] + m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m[3][0] * matrix.m[0][2] + m[3][1] * matrix.m[1][2] + m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		*this = matrix;
	}

	friend Vector3D operator * (Vector3D i, Matrix4x4 m) {
		Vector3D v;
		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];

		if (v.w) {
			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;
		}
		return v;
	}
};