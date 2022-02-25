/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами

*/

#include "linal/linal.h"
#include <vector>

class Figure {
public:
	virtual bool check_intersection(Ray r) = 0;
};

class Triangle : public Figure {
public:
	Vec3 v1, v2, v3;
    Vec3 n1, n2, n3;
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n1, Vec3 n2, Vec3 n3);
	bool check_intersection(Ray r);
};


class Sphere : public Figure {
    std::vector<Triangle> body;
public:
    Sphere(Vec3 center, double radius, unsigned iterations = 2);
    bool check_intersection(Ray r);
    std::vector<Triangle> get_body();
};

