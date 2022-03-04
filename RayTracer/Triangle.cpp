#include "Triangle.h"

Triangle::Triangle(point3 _vertex1, point3 _vertex2, point3 _vertex3) {
	vertex1 = _vertex1;
	vertex2 = _vertex2;
	vertex3 = _vertex3;
}

bool Triangle::intersection(ray r) {
	vec3 e1 = vertex2 - vertex1;
	vec3 e2 = vertex3 - vertex1;
	vec3 T = r.origin() - vertex1;
	vec3 P = cross(r.direction(), e2);
	vec3 Q = cross(T, e1);

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
	return true;
}