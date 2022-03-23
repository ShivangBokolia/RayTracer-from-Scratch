#include "Triangle.h"

Triangle::Triangle(point3 _vertex1, point3 _vertex2, point3 _vertex3, Material _material) {
	vertex1 = _vertex1;
	vertex2 = _vertex2;
	vertex3 = _vertex3;
	material = _material;
}

//bool Triangle::intersection(ray r) {
//	vec3 e1 = vertex2 - vertex1;
//	vec3 e2 = vertex3 - vertex1;
//	vec3 T = r.origin() - vertex1;
//	vec3 P = cross(r.direction(), e2);
//	vec3 Q = cross(T, e1);
//
//	// Almost parallel
//	if (dot(P, e1) == 0) {
//		return false;
//	}
//
//	double f = 1 / (dot(P, e1));
//	auto w = f * (dot(Q, e2));
//	auto u = f * (dot(P, T));
//	auto v = f * (dot(Q, r.direction()));
//
//	if (w < 0) {
//		return false;
//	}
//	if (u < 0 || v < 0 || u + v > 1) {
//		return false;
//	}
//	return true;
//}

bool Triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 e1 = vertex2 - vertex1;
	vec3 e2 = vertex3 - vertex1;

	vec3 T = r.origin() - vertex1;
	vec3 P = cross(r.direction(), e2);
	vec3 Q = cross(T, e1);

	vec3 triangle_normal = unit_vector(cross(e1, e2));

	// Almost parallel
	if (dot(P, e1) == 0) {
		return false;
	}

	double f = 1 / (dot(P, e1));
	auto w = f * (dot(Q, e2));
	auto u = f * (dot(P, T));
	auto v = f * (dot(Q, r.direction()));

	if (w < 0) {
		return false;
	}
	if (u < 0 || v < 0 || u + v > 1) {
		return false;
	}

	auto point = r.at(w);

	// Getting the dimensions of the triangles:
	double height = std::max(abs(vertex1.z() - vertex2.z()), abs(vertex1.z() - vertex3.z()));
	double width = std::max(abs(vertex1.x() - vertex2.x()), abs(vertex1.x() - vertex3.x()));

	// Getting the texture points:
	auto u_texture = abs((point - vertex1).x() / width);
	auto v_texture = abs((point - vertex3).z() / height);

	rec.t = w;
	rec.normal = triangle_normal;
	rec.hit_point = r.at(w);

	int row = (int)(u_texture * 10);
	int col = (int)(v_texture * 10);

	if ((row + col) % 2 == 0) {
		rec.obj_material = Material(0, color(1, 0, 0));
		//rec.obj_material = color(1, 0, 0);
	}
	else {
		rec.obj_material = Material(0, color(1, 1, 0));
		//rec.obj_material = color(1, 1, 0);
	}

	return true;
}