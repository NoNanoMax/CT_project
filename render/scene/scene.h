/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами

*/

#include "linal/linal.h"
#include <vector>

// figures -------------------------------

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

// lights -------------------------------

class Light_source {
public:
    Light_source();
    virtual double get_intense(Vec3 camera, Vec3 normal, int alpha) = 0;
    virtual double intense() = 0;
protected:  
    inline double get_intence_in(const Vec3 camera, const Vec3 normal, const Vec3& direction, int alpha);
};

class Ambient_light: public Light_source {
protected:
    double intens;
public: 
    Ambient_light(double inte);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Point_light: public Light_source {
protected:    
    Vec3 pos;
    double intens;
public:
    Point_light(Vec3 pos, double intens);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Directed_liht: public Light_source {
protected:
    Vec3 direction;
    double intens;
public:
    Directed_liht(Vec3 dir, double inte);
    double intense();
    double get_intense(Vec3 camera, Vec3 normal, int alpha);
};

class Light {
    std::vector<Light_source*> light;
    double sum_intense;
public:
    Light();
    ~Light();
    bool push_back(Light_source* source);
    double get_light(Vec3 camera, Vec3 point, Vec3 normal, int alpha = -1);
};


// camera -------------------------------




