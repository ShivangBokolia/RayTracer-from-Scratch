#include<iostream>
#include "Color.h"
#include "Ray.h"
#include "vec3.h"
#include "Sphere.h"
#include "Triangle.h"
#include "World.h"
#include "Camera.h"
#include "Phong.h"
#include "Material.h"


const double infinity = std::numeric_limits<double>::infinity();

//color phong_shading(const hit_record& rec, Light& light, Camera& camera, const Phong& phong) {
//	color diffuse_color = color();
//	color specular_color = color();
//	color ambient_color = phong.ka * rec.obj_material.mat_color;
//
//	vec3 light_dir = unit_vector(light.light_position - rec.hit_point);
//	vec3 camera_dir = unit_vector(camera.get_position() - rec.hit_point);
//
//	vec3 light_camera_dir = unit_vector(light_dir + camera_dir);
//
//	double light_distance = (light.light_position - rec.hit_point).length();
//	double NdotL = dot(rec.normal, light_dir);
//	if (NdotL >= 0) {
//		diffuse_color += (rec.obj_material.mat_color * light.light_color * (light.intensity * NdotL) * phong.kd);
//		//diffuse_color += (rec.obj_material * light.light_color * (light.intensity * NdotL) * phong.kd);
//	}
//
//	double RdotV = dot(rec.normal, light_camera_dir);
//	if (RdotV >= 0)
//	{
//		double specular = pow(RdotV, phong.ke);
//		specular_color += (light.light_color * light.intensity * specular * phong.ks);
//	}
//
//	return ambient_color + diffuse_color + specular_color;
//}

color lighting(hit_record& rec, const ObjectsHit& world, Phong& phong, Camera& camera) {
	color pixel_color = color(0, 0, 0);

	for (int i = 0; i < phong.lights.size(); i++) {
		Light light = phong.lights[i];

		vec3 light_dir = unit_vector(light.light_position - rec.hit_point);

		point3 shadow_position = rec.hit_point + (light_dir * 0.001);
		ray shadow_ray(shadow_position, light_dir);
		hit_record shadow_rec;

		if (world.hit(shadow_ray, 0, infinity, shadow_rec))
		{
			continue;
		} 
		pixel_color += phong.phong_shading(rec, light, camera);
	}
	return pixel_color;
}

color ray_color(const ray& r, const ObjectsHit& world, Phong& phong, Camera& camera, double depth) {
	hit_record rec;
	color pixel_color = color();

	if (depth <= 0) {
		return color();
	}

	if (world.hit(r, 0.001, infinity, rec))
	{
		double kr = rec.obj_material.get_kr();
		if (kr > 0) {
			vec3 reflection_vec = reflect(unit_vector(r.direction()), rec.normal);
			ray reflection_ray(rec.hit_point, reflection_vec);
			pixel_color += kr * ray_color(reflection_ray, world, phong, camera, depth-1);
		}
		else {
			pixel_color += lighting(rec, world, phong, camera);
		}
		return pixel_color;
	}
	else {
		auto t = 0.5 * (unit_vector(r.direction()).y() + 1);
		return (1 - t) * color(0.3, 0, 0.3) + t * color(0.5, 0.7, 1.0);
		//return color(0.5, 0.7, 1.0);
	}
}

int main()
{
	// Image Features:
	const int image_width = 600;
	const double aspect_ratio = 16.0 / 9.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;

	// Creating Materials for the objects:
	Material reflective_sphere(0.9, color(0.5, 0.5, 0.5));
	Material plain_sphere(color(1, 0, 0), color(1, 1, 1));
	Material plain_triangle(color(1, 0, 0), color(1, 1, 1));

	// World:
	World world;
	world.addObject(new Triangle(point3(-3, -1, -8), point3(8, -1, -8), point3(8, -1, 17), plain_triangle));
	world.addObject(new Triangle(point3(-3, -1, -8), point3(8, -1, 17), point3(-3, -1, 17), plain_triangle));
	world.addObject(new Sphere(point3(0, 3, 8), 2, reflective_sphere));
	world.addObject(new Sphere(point3(3, 2, 4), 1.75, reflective_sphere));

	// Lights:
	Phong phong(0.8, 1.0, 1.0, 1.0);
	phong.add_light(vec3(-4, 25, 30), color(1, 1, 1), 1);

	// Camera Features:
	Camera camera(point3(0, 5, 25), point3(0, 0, 0), vec3(0, 1, 0), 45, aspect_ratio);

	// Ray Tracing:
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < image_height; i++) {
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j=0; j< image_width; j++) {
			color pixel_color;
			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = double(j + random_double()) / (image_width - 1);
				auto v = double(i + random_double()) / (image_height - 1);
				ray r = camera.send_ray(u, v);
				pixel_color += ray_color(r, world, phong, camera, 50);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}