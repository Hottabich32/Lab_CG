#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Math3D.hpp"

struct Triangle {
	std::vector<Vector3D> p;
	Vector3D normal;

	Triangle() { p = std::vector<Vector3D>(3); }
	Triangle(Vector3D p1, Vector3D p2, Vector3D p3) {
		p = std::vector<Vector3D>(3);
		p[0] = p1;
		p[1] = p2;
		p[2] = p3;
	}
};

class Model {
public:
	std::vector<Triangle> tris;

	bool load_from_file(const char* file_name) {
		tris.clear();
		std::ifstream file(file_name);
		if (!file.is_open()) return false;
		std::vector<Vector3D> verts;

		while (!file.eof())
		{
			char line[128];
			file.getline(line, 128);
			std::stringstream s;
			s << line;

			char j = 0;
			if (line[0] == 'v')
			{
				Vector3D v;
				s >> j >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
			if (line[0] == 'f')
			{
				std::vector<int> f(3);
				bool has_slash = false;
				for (auto p : line)
				{
					if (p == '/')
					{
						has_slash = true;
						break;
					}
				}
				if (has_slash)
				{
					int foo;
					s >> j >> f[0] >> j >> foo >> f[1] >> j >> foo >> f[2] >> j >> foo;
				}
				else
				{
					s >> j >> f[0] >> f[1] >> f[2];
				}
				tris.push_back(Triangle(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]));
			}
		}
		return true;
	}
};