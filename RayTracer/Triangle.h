#pragma once
#include "vec3.h"
#include "Ray.h"
#include "ObjectsHit.h"

class Triangle {

private:
	point3 vertex1;
	point3 vertex2;
	point3 vertex3;

public:
	Triangle(point3 _vertex1, point3 _vertex2, point3 _vertex3);

	point3 getVertex1() const { return vertex1; }
	point3 getVertex2() const { return vertex2; }
	point3 getVertex3() const { return vertex3; }

	bool intersection(ray r);
};
