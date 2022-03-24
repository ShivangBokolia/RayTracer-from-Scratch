#pragma once

#include "vec3.h"
#include<iostream>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {

    auto scale = 1.0 / samples_per_pixel;
    double r = pixel_color.x() * scale;
    double g = pixel_color.y() * scale;
    double b = pixel_color.z() * scale;

	out << static_cast<int>(256 * clamp(sqrt(r), 0.0, 1.0)) << ' '
        << static_cast<int>(256 * clamp(sqrt(g), 0.0, 1.0)) << ' '
        << static_cast<int>(256 * clamp(sqrt(b), 0.0, 1.0)) << '\n';
}
