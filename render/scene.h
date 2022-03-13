/*

    scene.h

Здесь обсчитывается взаимодействие лучей с фигурами b источниками света

*/
#define WIDTH 2000
#define HEIGHT 1000

#include <map>
#include <fstream>
#include <sstream>
#include <cassert>
#include <sstream>
#include <cassert>
#include <stdio.h>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "linal/linal.h"
#include "material/material.h"
#include <vector>
#include "math.h"


enum types{CAMERA, LIGHT, FIGURE};

class Object{
public:
    Object();
    virtual Object* clone(std::vector<std::string> const & arg) = 0;
    virtual std::pair<int, std::string> name() const = 0;
};

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

class Figure{
public:
    // virtual std::vector<Triangle> get_body() = 0;
    //returns point of intersection and normal else dir = (2, 0, 0)
    virtual Ray does_intersect(Ray ray) = 0;
    virtual Triangle get_intersection_triangle(Ray ray) = 0;
};

class Sphere : public Figure, public Object{
    std::vector<Triangle> body;
public:
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(Vec3 center, double radius, unsigned iterations = 2);
    std::vector<Triangle> get_body();
    Triangle get_intersection_triangle(Ray r);
    //returns point of intersection and normal else dir = (2, 0, 0)
    virtual Ray does_intersect(Ray ray);
};

class BeautifulSphere : public Figure , public Object {
    Vec3 position = Vec3(0,0,0);
    double radius = 0;
public:
    BeautifulSphere();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(Vec3 center, double radius);
    // std::vector<Triangle> get_body(); //hz zachem eto
    Triangle get_intersection_triangle(Ray r);
    //returns point of intersection and normal else dir = (2, 0, 0)
    virtual Ray does_intersect(Ray ray);
};


class BeautifulPlane : public Figure, public Object {
    Vec3 normal = Vec3(0,0,1);
    double d = 0;
public:
    BeautifulPlane();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(Vec3 normal, double d);
    virtual Ray does_intersect(Ray r);
    Triangle get_intersection_triangle(Ray r);
    
};
// ------------------------------- lights -------------------------------

// -- Абстрактный класс источников света --
class Light_source {
public:
    virtual double intensity_in_point(Vec3 point, Vec3 normal) = 0;
    virtual Vec3 get_position(Vec3 point) = 0;
};

class Ambient_light: public Light_source, public Object {
private:
    double intensity = 0;
public: 
    Ambient_light();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(double intensity);
    Vec3 get_position(Vec3 point);
    double intensity_in_point(Vec3 point, Vec3 normal);
};

class Point_light: public Light_source, public Object {
private:    
    Vec3 position = Vec3(0,0,0);
    double intensity = 0;
public:
    Point_light();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(Vec3 position, double intensity);
    Vec3 get_position(Vec3 point);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

class Directed_light: public Light_source, public Object {
private:
    Vec3 direction = Vec3(0,0,0);;
    double intensity = 0;
public:
    Directed_light();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    void build(Vec3 direction, double intensity);
    Vec3 get_position(Vec3 point);
    double intensity_in_point(Vec3 camera, Vec3 normal);
};

// ------------------------------- camera -------------------------------

// -- Класс камеры, камера смотрит вдоль оси y в собственной системе координат --
class Camera: public Object {
    Vec3 position;
    double angles[3]; // psi, theta, pfi;
    Matr3 rotate; // матрица поворота
    double FOV; // ширина поля зрения вдоль WIDTH: 1,57 - 90 Градусов
    double distance; // расстояние до экрана
    double width, height; // ширина и высота экрана
    Matr3 calculate_rotate();
    Ray calculate_ray(unsigned x, unsigned y);
public:
Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    Camera(); // пустой конструктор (источник в начале координат, углы нулевые, расстояние, ширина, высота единицы)
    Camera(Vec3 position, Vec3 angels, double distance = 1, double width = 1, double height = 1);
    std::vector<std::vector<Ray>> create_rays(); // генерация лучей
};

// ------------------------------- scene -------------------------------

class Scene {
    std::vector<Figure*> figures;
    std::vector<Light_source*> lights;
 
    Camera* camera = nullptr;
    Color** res;
public:
    Scene();
    ~Scene();
    Scene(std::vector<Figure*> figures, std::vector<Light_source*> lights, Camera* camera);

    Color** get_res();

    //fills scene with objects
    void initialization(const char * input);

    //calculates everything
    void render();
    
    //inner methods
    Color trace_ray(Ray ray);
    double full_intensity_in_point(Vec3 point, Vec3 normal);
    Ray get_first_intersection(Ray ray);
};
