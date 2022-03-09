#pragma once
#include "vec3.h"
#include "Ray.h"
#include "ObjectsHit.h"

class Sphere : public ObjectsHit {

private:
	double radius;
	point3 center;
	color material;

public:
	Sphere(point3 _center, double _radius, color _material);

	double get_radius() const { return radius; }
	point3 get_center() const { return center; }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};