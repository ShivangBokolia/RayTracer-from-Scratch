#pragma once

#include "vec3.h"

struct Light {
	color light_color;
	double intensity;
	vec3 light_position;

	Light(color _light_color, double _intensity, vec3 _light_position) {
		light_color = _light_color;
		intensity = _intensity;
		light_position = _light_position;
	}
};

