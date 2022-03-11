/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами b источниками света

*/

#define WIDTH 600
#define HEIGHT 600

#include "linal/linal.h"
#include "material/material.h"
#include <vector>
#include "math.h"

// ------------------------------- figures -------------------------------

class Triangle {
public:
	Vec3 v1, v2, v3;
    Vec3 n1, n2, n3;
    Vec3 N; // вектор нормали (к плоскости)
    double D; // коэффициент в уравнении плоскости
    Vec3 U, V; // вспомогательные вектора для рассчёта пересечений
    double u0, v0; // вспомогательные величины для рассчёта пересечений
    Triangle();
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n1, Vec3 n2, Vec3 n3);
	bool check_intersection(Ray r);
    Vec3 get_intersection_point(Ray r);
    Vec3 get_normal(Vec3 point);
    bool is_empty(); // плохой ли треугольник
};

class Figure {
public:
    virtual std::vector<Triangle> get_body() = 0;
    //returns point of intersection and normal else dir = (2, 0, 0)
    virtual Ray does_intersect(Ray ray) = 0;
    virtual Triangle get_intersection_triangle(Ray ray) = 0;
};

class Sphere : public Figure {
    std::vector<Triangle> body;
public:
    void build(Vec3 center, double radius, unsigned iterations = 2);
    std::vector<Triangle> get_body();
    Triangle get_intersection_triangle(Ray r);
    //returns point of intersection and normal else dir = (2, 0, 0)
    virtual Ray does_intersect(Ray ray);
};

// ------------------------------- lights -------------------------------

// -- Абстрактный класс источников света --
class Light_source {
public:
    virtual double intensity_in_point(Vec3 point, Vec3 normal) = 0;
};

class Ambient_light: public Light_source {
private:
    double intensity;
public: 
    Ambient_light();
    void build(double intensity);
    double intensity_in_point(Vec3 point, Vec3 normal);
};

class Point_light: public Light_source {
private:    
    Vec3 position;
    double intensity;
public:
    Point_light();
    void build(Vec3 position, double intensity);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

class Directed_light: public Light_source {
private:
    Vec3 direction;
    double intensity;
public:
    Directed_light();
    void build(Vec3 direction, double intensity);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

// ------------------------------- camera -------------------------------

// -- Класс камеры, камера смотрит вдоль оси y в собственной системе координат --
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
    Camera(); // пустой конструктор (источник в начале координат, углы нулевые, расстояние, ширина, высота единицы)
    Camera(Vec3 position, double* angels, double distance = 1, double width = 1, double height = 1, unsigned width_pixels = WIDTH, unsigned height_pixels = HEIGHT);
    std::vector<Ray> create_rays(); // генерация лучей
};

// ------------------------------- scene -------------------------------

class Scene {
    std::vector<Figure*> figures;
    std::vector<Light_source*> lights;
 
    Camera camera;
    Color** res;
public:
    Scene();
    ~Scene();
    Scene(std::vector<Figure*> figures, std::vector<Light_source*> lights, Camera camera);

    Color** get_res();

    //fills scene with objects
    void initialization(const char * input);

    //calculates everything
    void render();
    
    //inner methods
    void trace_ray(Ray ray);
    double full_intensity_in_point(Vec3 point, Vec3 normal);
    Ray get_first_intersection(Ray ray);
};


