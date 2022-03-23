#pragma once
#include "vec3.h"
#include "Ray.h"
#include "ObjectsHit.h"
#include "Material.h"

class Sphere : public ObjectsHit {

private:
	double radius;
	point3 center;
	Material material;

public:
	const double infinity = std::numeric_limits<double>::infinity();

	Sphere(point3 _center, double _radius, Material _material);

	double get_radius() const { return radius; }
	point3 get_center() const { return center; }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};