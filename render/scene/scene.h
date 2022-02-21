/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами

*/

#include "linal/linal.h"

class Figure {
public:
	virtual bool check_intersection(Ray r) = 0; // { return true; }
};

class Triangle : public Figure {
private:
	Vec3 v1, v2, v3;
public:
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3);
	bool check_intersection(Ray r) override;
};



