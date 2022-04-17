#include "Phong.h"

void Phong::add_light(vec3 light_position, color light_color, double intensity) {
	lights.push_back(Light(light_color, intensity, light_position));
}

color Phong::ambient_lighting(hit_record& rec) {
	color ambient_color = unit_vector(ka * rec.obj_material.get_diffuse_color());
	return ambient_color;
}

color Phong::phong_shading(hit_record& rec, Light& light, Camera& camera) {
	color diffuse_color = color();
	color specular_color = color();

	vec3 light_dir = unit_vector(light.light_position - rec.hit_point);
	vec3 camera_dir = unit_vector(camera.get_position() - rec.hit_point);

	vec3 light_reflect_vec = unit_vector(reflect(light_dir, rec.normal));
	double RdotV = dot(light_reflect_vec, camera_dir);

	double NdotL = dot(rec.normal, light_dir);
	if (NdotL >= 0) {
		diffuse_color += (rec.obj_material.get_diffuse_color() * light.light_color * (light.intensity * NdotL) * kd);
	}

	if (RdotV >= 0)
	{
		specular_color += (ks * rec.obj_material.get_specular_color() * pow(RdotV, ke) * light.intensity * light.light_color);
	}

	return diffuse_color + specular_color;
}