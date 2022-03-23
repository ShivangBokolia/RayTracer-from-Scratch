#include "Phong.h"

void Phong::add_light(vec3 light_position, color light_color, double intensity) {
	lights.push_back(Light(light_color, intensity, light_position));
}

color Phong::phong_shading(const hit_record& rec, Light& light, Camera& camera) {
	color diffuse_color = color();
	color specular_color = color();

	vec3 light_dir = unit_vector(light.light_position - rec.hit_point);
	vec3 camera_dir = unit_vector(camera.get_position() - rec.hit_point);

	vec3 light_camera_dir = unit_vector(light_dir + camera_dir);

	double light_distance = (light.light_position - rec.hit_point).length();
	double NdotL = dot(rec.normal, light_dir);
	if (NdotL >= 0) {
		diffuse_color += (rec.obj_material.mat_color * light.light_color * (light.intensity * NdotL) * kd);
	}

	double RdotV = dot(rec.normal, light_camera_dir);
	if (RdotV >= 0)
	{
		double specular = pow(RdotV, ke);
		specular_color += (light.light_color * light.intensity * specular * ks);
	}

	return diffuse_color + specular_color;
}