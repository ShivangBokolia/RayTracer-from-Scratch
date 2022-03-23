#pragma once

#include "vec3.h"

class Material {

public:
	double kr;
	color mat_color;

public:
	Material() : kr(0), mat_color(color(0, 0, 0)) {}
	Material(double _kr, color _mat_color) : kr(_kr), mat_color(_mat_color) {}

	double get_kr() { return kr; }
	color get_mat_color() { return mat_color; }
};
