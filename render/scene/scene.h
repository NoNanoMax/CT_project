/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами

*/

#include "linal/linal.h"
#include <vector>
#include "math.h"

// ------------------------------- figures -------------------------------

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

// ------------------------------- lights -------------------------------

// -- абстрактный класс источников света --
class Light_source {
public:
    virtual double intensity_in_point(Vec3 point, Vec3 normal) = 0;
};

// -- класс, управляющий источниками света --
class Lights {
private:
    std::vector<Light_source*> sources;
public:
    Lights();
    void push_back(Light_source* source);
    double full_intensity_in_point(Vec3 point, Vec3 normal);
};

class Ambient_light: public Light_source {
private:
    double intensity;
public: 
    Ambient_light(double intensity);
    double intensity_in_point(Vec3 point, Vec3 normal);
};

class Point_light: public Light_source {
private:    
    Vec3 position;
    double intensity;
public:
    Point_light(Vec3 position, double intensity);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

class Directed_light: public Light_source {
private:
    Vec3 direction;
    double intensity;
public:
    Directed_light(Vec3 direction, double intensity);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

// ------------------------------- camera -------------------------------

// класс камеры, камера смотрит вдоль оси y в собственной системе координат
class Camera {
    Vec3 position;
    double angles[3]; // psi, theta, pfi;
    Matr3 rotate; // матрица поворота
    double distance; // расстояние до экрана
    double width, height; // ширина и высота экрана
    unsigned width_pixels, height_pixels; // число пикселей по горизонтали и вертикали
    Matr3 calculate_rotate();
    Ray calculate_ray(unsigned x, unsigned y);
public:
    Camera(Vec3 position, double* angels, double distance = 1, double width = 1, double height = 1, unsigned width_pixels = 1280, unsigned height_pixels = 720);
    std::vector<std::vector<Ray> > create_rays();
};

// ------------------------------- scene -------------------------------

class Scene {
    std::vector<Figure*> figures;
    Lights lights;


/*
    Vec3 trace_ray(const Scene& const Sc, const Light const Li, 
                Vec3 begin, Vec3 vec, double min_t,double max_t) const{
    Object A;
    Vec3 point;
    Sc.intersect(begin, vec, min_t, max_t, &A, &point);
    double light = Li.get_light(Sc, begin, point, A.normal(point), A.alpha());

}*/
};


