#include "World.h"

void World::addObject(ObjectsHit* obj) {
	objects.push_back(obj);
}

bool World::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool any_hit_occured = false;
	auto closest = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest, temp_rec)) {
			if (temp_rec.t < closest)
			{
				closest = temp_rec.t;
				rec = temp_rec;
				any_hit_occured = true;
			}
		}
	}
	return any_hit_occured;
}