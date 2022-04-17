#pragma once

#include "vec3.h"

class Material {

private:
	double kr;
	double kt;
	double ref_index;
	color diffuse_color;
	color specular_color;

public:
	Material() : kr(0), kt(0), ref_index(1), diffuse_color(color(0, 0, 0)) {}
	Material(color _diffuse_color, color _specular_color) : kr(0), kt(0), ref_index(1), diffuse_color(_diffuse_color), specular_color(_specular_color) {}
	Material(double _kr, double _kt, double _ref_index, color _diffuse_color) : kr(_kr), kt(_kt), ref_index(_ref_index), diffuse_color(_diffuse_color) {}

	double get_kr() { return kr; }
	double get_kt() { return kt; }
	double get_ref_index() { return ref_index; }
	color get_diffuse_color() { return diffuse_color; }
	color get_specular_color() { return specular_color; }
};
