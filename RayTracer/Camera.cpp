#include "Camera.h"

Camera::Camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio) {
	// Calculating theta in radians
	auto theta = vfov * (pi / 180);
	auto h = tan(theta / 2);
	auto viewport_height = 2.0 * h;
	auto viewport_width = aspect_ratio * viewport_height;

	auto w = unit_vector(lookfrom - lookat);
	auto u = unit_vector(cross(vup, w));
	auto v = cross(w, u);

	origin = lookfrom;
	horizontal = viewport_width * u;
	vertical = viewport_height * v;
	top_left_corner = origin - (horizontal / 2) + (vertical / 2) - w;
}

point3 Camera::get_position() {
	return origin;
}

ray Camera::send_ray(double s, double t) {
	auto ray_vec = unit_vector(top_left_corner + (s * horizontal) - (t * vertical) - origin);
	return ray(origin, ray_vec);
}