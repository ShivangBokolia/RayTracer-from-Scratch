#include "Sphere.h"

Sphere::Sphere(point3 _center, double _radius, color _material) {
	center = _center;
	radius = _radius;
	material = _material;
}

//double Sphere::intersection(ray r) {
//	vec3 oc = r.origin() - get_center();
//
//	// The old equations for the circle, they are further improved below, but do the same calculations.
//	/*auto a = dot(r.direction(), r.direction());
//	auto b = 2.0 * dot(oc, r.direction());
//	auto c = dot(oc, oc) - (get_radius() * get_radius());
//	auto discriminant = (b * b) - (4 * a * c);*/
//
//	auto a = r.direction().length_squared();
//	auto half_b = dot(oc, r.direction());
//	auto c = oc.length_squared() - (get_radius() * get_radius());
//	auto discriminant = (half_b * half_b) - (a * c);
//
//	if (discriminant < 0)
//	{
//		return -1.0;
//	}
//	else {
//		return (-half_b - sqrt(discriminant)) / a;
//	}
//}

bool Sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - get_center();

	// The old equations for the circle, they are further improved below, but do the same calculations.
	/*auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - (get_radius() * get_radius());
	auto discriminant = (b * b) - (4 * a * c);*/

	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - (get_radius() * get_radius());
	
	auto discriminant = (half_b * half_b) - (a * c);
	
	if (discriminant < 0) return false;

	auto root = (-half_b - sqrt(discriminant)) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrt(discriminant)) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}

	rec.t = root;
	rec.hit_point = r.at(rec.t);
	rec.normal = (rec.hit_point - get_center()) / get_radius();
	rec.obj_material = material;
	return true;
}