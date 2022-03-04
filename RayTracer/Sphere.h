#pragma once
#include "vec3.h"
#include "Ray.h"

class Sphere {

private:
	double radius;
	point3 center;

public:
	Sphere(point3 _center, double _radius);

	double get_radius() const { return radius; }
	point3 get_center() const { return center; }

	bool intersection(ray r);
};
