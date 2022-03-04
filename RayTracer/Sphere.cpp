#include "Sphere.h"

Sphere::Sphere(point3 _center, double _radius) {
	center = _center;
	radius = _radius;
}

bool Sphere::intersection(ray r) {
	vec3 oc = r.origin() - get_center();
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - (get_radius() * get_radius());
	auto discriminant = (b * b) - (4 * a * c);
	return (discriminant > 0);
}