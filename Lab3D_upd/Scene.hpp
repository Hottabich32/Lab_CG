#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Model.hpp"
#include "Camera.hpp"

class Scene {
	Camera camera;
	Model model;
	Vector3D light_dir;
	float angle_y = 0;   // текущий угол поворота модели вокруг Y

	// Применяет к треугольнику преобразования модели, камеру и проекцию.
	// Возвращает false, если грань нелицевая.
	bool transform_triangle(const Triangle& src, Triangle& out) const
	{
		for (int k = 0; k < 3; k++)
		{
			out.p[k] = src.p[k] * Matrix4x4::get_scale(1.0f, 1.0f, 1.0f);
			out.p[k] = out.p[k] * Matrix4x4::get_rot_y(angle_y * 1.5f);          
			out.p[k] = out.p[k] * Matrix4x4::get_rot_z(3.1415f);         
			out.p[k] = out.p[k] * Matrix4x4::get_translate(0.0f, 0.0f, 0.0f);
		}

		out.normal = cross_prod(out.p[2] - out.p[0], out.p[1] - out.p[0]);
		out.normal.norm();

		Vector3D cam_dir = (out.p[0] + out.p[1] + out.p[2]) / 3 - camera.get_position();
		if (dot_prod(out.normal, cam_dir) < 0)
			return false;

		for (int k = 0; k < 3; k++)
		{
			out.p[k] = out.p[k] * camera.get_view();
			out.p[k] = out.p[k] * camera.get_proj();
		}
		return true;
	}

public:
	Scene(const Camera& cam, Vector3D light = Vector3D(1, -0.5, -0.7))
		: camera(cam), light_dir(light)
	{
		light_dir.norm();
	}

	Model& get_model() { return model; }
	Camera& get_camera() { return camera; }

	void rotate(float delta) { angle_y += delta; }

	void render(sf::RenderTarget& target) const
	{
		const sf::Vector2u size = target.getSize();

		std::vector<Triangle> to_draw;
		for (const auto& t : model.tris)
		{
			Triangle tri;
			if (transform_triangle(t, tri))
				to_draw.push_back(tri);
		}

		// алгоритм художника: дальние грани рисуются первыми
		std::sort(to_draw.begin(), to_draw.end(), [](const Triangle& t1, const Triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			});

		for (const auto& T : to_draw)
		{
			float light = 0.3f + 0.7f * std::max(0.0f, dot_prod(-T.normal, light_dir));
			sf::Color color(153 * light, 249 * light, 142 * light);

			sf::VertexArray tri(sf::PrimitiveType::Triangles, 3);
			for (int j = 0; j < 3; j++)
			{
				tri[j].position = sf::Vector2f((T.p[j].x + 1) * size.x / 2, (T.p[j].y + 1) * size.y / 2);
				tri[j].color = color;
			}
			target.draw(tri);
		}
	}
};