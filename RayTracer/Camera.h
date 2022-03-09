#pragma once
#include "vec3.h"
#include "Ray.h"

class Camera {

private:
	point3 origin;
	vec3 horizontal;
	vec3 vertical;
	point3 top_left_corner;

public:
	// Takes the vertical field of view and the aspect ratio.
	Camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio);

	point3 get_position();
	ray send_ray(double u, double v);
};
