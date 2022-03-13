#pragma once
#include "vec3.h"
#include "Ray.h"
#include "ObjectsHit.h"

class Triangle : public ObjectsHit {

private:
	point3 vertex1;
	point3 vertex2;
	point3 vertex3;
	color material;

public:
	Triangle(point3 _vertex1, point3 _vertex2, point3 _vertex3, color _material);

	point3 getVertex1() const { return vertex1; }
	point3 getVertex2() const { return vertex2; }
	point3 getVertex3() const { return vertex3; }
	color getMaterial() const { return material; }

	bool intersection(ray r);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
};
