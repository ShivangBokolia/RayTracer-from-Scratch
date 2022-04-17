#pragma once
#include <memory>
#include <vector>
#include "ObjectsHit.h"

using namespace std;

class World : public ObjectsHit{

public:
	void addObject(ObjectsHit* obj);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	

public:
	vector<ObjectsHit*> objects;
};
