#pragma once
#include "Math3D.hpp"

class Camera {
	Vector3D position;
	Vector3D target;
	Vector3D up;
	Matrix4x4 view_mat;
	Matrix4x4 proj_mat;

	void update_view() {
		view_mat = Matrix4x4::get_point_at_mat(position, target, up);
		view_mat.invert();
	}

public:
	Camera(Vector3D pos, Vector3D look_at, Vector3D up_dir,
		int width, int height, float z_near = 0.1f, float z_far = 1000, float fov = 90)
		: position(pos), target(look_at), up(up_dir)
	{
		proj_mat = Matrix4x4::get_proj_mat(width, height, z_near, z_far, fov);
		update_view();
	}

	void set_position(Vector3D pos) { position = pos; update_view(); }
	void set_target(Vector3D look_at) { target = look_at; update_view(); }

	Vector3D get_position() const { return position; }
	const Matrix4x4& get_view() const { return view_mat; }
	const Matrix4x4& get_proj() const { return proj_mat; }
};