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

color lighting(hit_record& rec, const ObjectsHit& world, Phong& phong, Camera& camera) {
	color pixel_color = color(0, 0, 0);
	/*color pixel_color = (phong.ambient_lighting(rec) * color(0.5, 0.7, 1.0));*/

	for (int i = 0; i < phong.lights.size(); i++) {
		Light light = phong.lights[i];

		vec3 light_dir = unit_vector(light.light_position - rec.hit_point);

		point3 shadow_position = rec.hit_point + (light_dir * 0.001);
		ray shadow_ray(shadow_position, light_dir);
		hit_record shadow_rec;

		double shadow_multiplier = 1.0;
		if (world.hit(shadow_ray, 0.001, infinity, shadow_rec))
		{
			shadow_multiplier = 1 - shadow_rec.obj_material.get_kr();
			if (shadow_multiplier <= 0)
			{
				continue;
			}
		}
		pixel_color += phong.phong_shading(rec, light, camera) * shadow_multiplier;
		
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
		double kt = rec.obj_material.get_kt();

		double n2 = rec.obj_material.get_ref_index();
		double refractive_index = 1 / n2;

		vec3 reflection_vec = reflect(unit_vector(-r.direction()), rec.normal);

		if (kr > 0) {
			ray reflection_ray(rec.hit_point, reflection_vec);
			pixel_color += kr * ray_color(reflection_ray, world, phong, camera, depth-1);
		}
		if (kt > 0) {
			vec3 normal = rec.normal;
			if (dot(normal, -r.direction()) < 0)
			{
				normal *= -1;
				refractive_index = n2 / 1;
			}
			vec3 refraction_vec = refract(r.direction(), normal, refractive_index);

			ray refraction_ray(rec.hit_point + 0.01 * refraction_vec, refraction_vec);
			pixel_color += kt * ray_color(refraction_ray, world, phong, camera, depth - 1);
			
		}
		else {
			pixel_color += lighting(rec, world, phong, camera);
		}

		return pixel_color;
	}
	else {
		auto t = 0.5 * (unit_vector(r.direction()).y() + 0.5);
		return (1 - t) * color(0.3, 0, 0.3) + t * color(0.5, 0.7, 1.0);
		/*return color(0.5, 0.7, 1.0);*/
	}
}

double calculate_log_average_lum( double** luminance, int rows, int cols ) {
	double log_average = 0;
	double delta = 0.00001;

	for (int i = 0;i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			log_average += log10(delta + luminance[i][j]);
		}
	}
	log_average = log_average / ((double)rows * (double)cols);
	log_average = exp(log_average);

	return log_average;
}

double ward_tone_reproduction(double lwa, double ld_max) {
	double top = 1.219 + pow((ld_max / 2), 0.4);
	double bottom = 1.219 + pow(lwa, 0.4);
	double sf = pow((top / bottom), 2.5);

	return sf;
}

void adaptive_log_mapping(double lwa, double ld_max, color**& pixel_grid, double**& luminance, double rows, double cols) {
	ld_max = ld_max / lwa;
	double first_bottom = log10(ld_max + 1);
	double b = 0.95;
	double power = log(b) / log(0.5);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			double lw = luminance[i][j] / lwa;
			double second_top = log(lw + 1);
			double lw_frac = lw / ld_max;
			double second_bottom = log(2 + ((pow(lw_frac, power) * 8)));

			double ld = (1 / first_bottom) * (second_top / second_bottom);
			pixel_grid[i][j] *= ld;
		}
	}
	
}

int main()
{
	// Image Features:
	const int image_width = 720;
	const double aspect_ratio = 16.0 / 9.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;

	// Creating Materials for the objects:
	// kr, kt, ref_index
	Material reflective_sphere(0.9, 0, 1.0, color(0.5, 0.5, 0.5));
	Material transmittive_sphere(0.1, 0.9, 0.97, color(1, 1, 1));
	Material plain_sphere(color(1, 0, 0), color(1, 1, 1));
	Material plain_sphere_2(color(0, 1, 0), color(1, 1, 1));
	Material plain_triangle(color(1, 0, 0), color(1, 1, 1));

	// World:
	World world;
	world.addObject(new Triangle(point3(-3, -1, -8), point3(8, -1, -8), point3(8, -1, 17), plain_triangle));
	world.addObject(new Triangle(point3(-3, -1, -8), point3(8, -1, 17), point3(-3, -1, 17), plain_triangle));
	world.addObject(new Sphere(point3(0, 3, 8), 2, transmittive_sphere));
	world.addObject(new Sphere(point3(3, 2, 4), 1.75, reflective_sphere));

	// Lights:
	// ka, kd, ks, ke
	Phong phong(0.1, 0.5, 0.5, 128);
	phong.add_light(vec3(-4, 25, 30), color(1, 1, 1), 1);

	// Camera Features:
	/*Camera camera(point3(0, 5, 25), point3(0, 0, 0), vec3(0, 1, 0), 45, aspect_ratio);*/
	Camera camera(point3(0, 5, 20), point3(0, 0, 0), vec3(0, 1, 0), 45, aspect_ratio);

	// Tone Reproduction :
	double ld_max = -1;
	// ward, reinhard, avg (advanced) or none
	string whichTR = "none";

	// Ray Tracing:
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	color** pixel_color_grid = new color* [image_height];
	for (int i = 0; i < image_height; i++)
	{
		pixel_color_grid[i] = new color[image_width];
	}

	double** pixel_color_luminance = new double* [image_height];
	for (int i = 0; i < image_height; i++)
	{
		pixel_color_luminance[i] = new double[image_width];
	}

	for (int i = 0; i < image_height; i++) {
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j=0; j< image_width; j++) {
			color pixel_color;
			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = double(j + random_double()) / (image_width - 1);
				auto v = double(i + random_double()) / (image_height - 1);
				ray r = camera.send_ray(u, v);
				pixel_color += ray_color(r, world, phong, camera, 10);
			}
			pixel_color_grid[i][j] = pixel_color;
			pixel_color_luminance[i][j] = (0.27 * pixel_color.x()) + (0.67 * pixel_color.y()) + (0.06 * pixel_color.z());

			if (ld_max < pixel_color_luminance[i][j])
			{
				ld_max = pixel_color_luminance[i][j];
			}
		}
	}

	double la = calculate_log_average_lum(pixel_color_luminance, image_height, image_width);

	if (whichTR == "ward")
	{
		double sf = ward_tone_reproduction(la, ld_max);
		for (int i = 0; i < image_height; i++)
		{
			for (int j = 0; j < image_width; j++)
			{
				pixel_color_grid[i][j] *= sf;
			}
		}
	}
	else if (whichTR == "reinhard") {
		for (int i = 0; i < image_height; i++)
		{
			for (int j = 0; j < image_width; j++)
			{
				pixel_color_grid[i][j] *= (0.18 / la);
				color temp = pixel_color_grid[i][j];
				color new_value = color(temp.x() / (1 + temp.x()), temp.y() / (1 + temp.y()), temp.z() / (1 + temp.z()));
				pixel_color_grid[i][j] = new_value * ld_max;
			}
		}
	}
	else if (whichTR == "avg") {
		adaptive_log_mapping(la, ld_max, pixel_color_grid, pixel_color_luminance, image_height, image_width);
	}

	for (int i = 0; i < image_height; i++)
	{
		for (int j = 0; j < image_width; j++)
		{
			write_color(std::cout, pixel_color_grid[i][j], samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}