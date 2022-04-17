#pragma once
#include <vector>
#include "vec3.h"
#include "Light.h"
#include "ObjectsHit.h"
#include "Camera.h"

using namespace std;


class Phong {
public:
	double ka;
	double kd;
	double ks;
	double ke;

public:
	Phong(double _ka, double _kd, double _ks, double _ke) : ka(_ka), kd(_kd), ks(_ks), ke(_ke) {};

	void add_light(vec3 light_position, color light_color, double intensity);

	color phong_shading(hit_record& rec, Light& light, Camera& camera);

	color ambient_lighting(hit_record& rec);

	color blinn_phong_shading(hit_record& rec, Light& light, Camera& camera);

public:
	vector<Light> lights;
};
