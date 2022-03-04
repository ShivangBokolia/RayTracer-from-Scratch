#include<iostream>
#include "Color.h"
#include "Ray.h"
#include "vec3.h"
#include "Sphere.h"
#include "Triangle.h"


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

	point3 origin = point3(0, 0, 1);
	vec3 horizontal = vec3(viewport_width, 0, 0);
	vec3 vertical = vec3(0, viewport_height, 0);
	auto upper_left_corner = origin - (horizontal / 2) + (vertical / 2) - vec3(0, 0, focal_length);

	// Sphere 1:
	Sphere sphere1(point3(0, 0, -1), 0.5);

	// Sphere 2:
	Sphere sphere2(point3(0.6, -0.5, -1.2), 0.4);

	// Triangle 1:
	Triangle triangle1(point3(-1, -1, -5), point3(5, -1, -5), point3(11, -6, -5));

	// Triangle 2:
	Triangle triangle2(point3(-1, -1, -5), point3(11, -6, -5), point3(-3, -6, -5));

	// Ray Tracing:
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i = 0; i < image_height; i++) {
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j=0; j< image_width; j++) {
			auto u = double(j) / (image_width - 1);
			auto v = double(i) / (image_height - 1);
			ray r(origin, upper_left_corner + (u * horizontal) - (v * vertical) - origin);
			color pixel_color = color(0.5, 0.7, 1.0);
			if (sphere1.intersection(r))
			{
				pixel_color = color(1.0, 0, 0.7);
			}
			else if (sphere2.intersection(r)) {
				pixel_color = color(0.7, 0, 1.0);
			}
			else if (triangle1.intersection(r)) {
				pixel_color = color(0, 1, 0);
			}
			else if (triangle2.intersection(r)) {
				pixel_color = color(0, 1, 0);
			}
			//color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}