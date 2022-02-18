#include<iostream>
#include "Color.h"
#include "Ray.h"
#include "vec3.h"

bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - (radius * radius);
	auto discriminant = (b * b) - (4 * a * c);
	return (discriminant > 0);
}

bool hit_triangle(const point3& p0, const point3& p1, const point3& p2, const ray& r) {
	vec3 e1 = p1 - p0;
	vec3 e2 = p2 - p0;
	vec3 T = r.origin() - p0;
	vec3 P = cross(r.direction(), e2);
	vec3 Q = cross(T, e1);

	if (dot(P, e1) == 0) {
		return false;
	}

	double f = 1 / (dot(P, e1));
	auto w = f * (dot(Q, e2));
	auto u = f * (dot(P, T));
	auto v = f * (dot(Q, r.direction()));

	if (w < 0) {
		return false;
	}
	if (u < 0 || v < 0 || u + v > 1) {
		return false;
	}
	return true;
}


color ray_color(const ray& ray) {
	if (hit_sphere(point3(0, 0, -1), 0.5, ray)) {
		return color(1, 0, 0);
	}
	if (hit_sphere(point3(0.6, -0.5, -1.2), 0.4, ray)) {
		return color(0, 0, 1);
	}
	if (hit_triangle(point3(-1, -1, -5), point3(5, -1, -5), point3(11, -6, -5), ray)) {
		return color(0, 1, 0);
	}
	if (hit_triangle(point3(-1, -1, -5), point3(11, -6, -5), point3(-3, -6, -5), ray)) {
		return color(0, 1, 0);
	}
	vec3 unit_direction = unit_vector(ray.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	// Image Features:
	const int image_width = 600;
	const double aspect_ratio = 16.0 / 9.0;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// Camera Features:
	double viewport_height = 2.0;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.0;

	point3 origin = point3(0, 0, 0);
	vec3 horizontal = vec3(viewport_width, 0, 0);
	vec3 vertical = vec3(0, viewport_height, 0);
	auto upper_left_corner = origin - (horizontal / 2) + (vertical / 2) - vec3(0, 0, focal_length);

	// Ray Tracing:
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < image_height; i++) {
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j=0; j< image_width; j++) {
			auto u = double(j) / (image_width - 1);
			auto v = double(i) / (image_height - 1);
			ray r(origin, upper_left_corner + (u * horizontal) - (v * vertical) - origin);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}