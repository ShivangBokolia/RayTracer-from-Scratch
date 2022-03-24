#pragma once

#include "vec3.h"

class Material {

private:
	double kr;
	color diffuse_color;
	color specular_color;

public:
	Material() : kr(0), diffuse_color(color(0, 0, 0)) {}
	Material(color _diffuse_color, color _specular_color) : kr(0), diffuse_color(_diffuse_color), specular_color(_specular_color) {}
	Material(double _kr, color _diffuse_color) : kr(_kr), diffuse_color(_diffuse_color) {}

	double get_kr() { return kr; }
	color get_diffuse_color() { return diffuse_color; }
	color get_specular_color() { return specular_color; }
};
