#pragma once
#include "Ray.h"
#include "Material.h"

struct hit_record {
	point3 hit_point;
	vec3 normal;
	double t;
	Material obj_material;
	std::string object_name;
	//color obj_material;
};

class ObjectsHit {

public:
	
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
