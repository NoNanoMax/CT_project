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
#include <vector>
#include "math.h"


enum types{CAMERA, LIGHT, FIGURE};

// Шаблонный класс для всех объектов сцены (камера / источники света / фигуры)
class Object{
public:
    Object() { }
    virtual Object* clone(std::vector<std::string> const & arg) = 0;
    virtual std::pair<int, std::string> name() const = 0;
};

// ------------------------------- Figures -------------------------------

// Умная точка, хранящая точку, нормаль, цвет, материал
class SmartPoint {
public:
    Vec3 point = Vec3(0, 0, 0);
    Vec3 normal = Vec3(0, 0, 0);
    bool valid = true;
    Color color = Color(255, 255, 255);
    Material material = Material("dielectric");
    SmartPoint(bool valid);
    SmartPoint(Vec3 point, Vec3 normal);
    SmartPoint(Vec3 point, Vec3 normal, Color color, Material material, bool valid = false);
};

class Figure {
public:
    // возвращает SmartPoint (супер точку) по лучу
    virtual SmartPoint get_intersection_SmartPoint(Ray ray) = 0;
};
/*
// ------------------------------- PolygonizedFigures -------------------------------

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

class PolygonizedFigure {
    std::vector<Triangle>* body;
public:
    PolygonizedFigure();
    ~PolygonizedFigure();
    SmartPoint get_intersection_SmartPoint(Ray r);
};

class Sphere : public Figure, public PolygonizedFigure, public Object {
public:
    Sphere();
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
};
*/
// ------------------------------- AnaliticFigures -------------------------------

class BeautifulSphere : public Figure, public Object {
    Vec3 position = Vec3(0, 0, 0);
    double radius = 0;
    Color color = Color(255, 255, 255);
    Material material = Material("dielectric");
public:
    BeautifulSphere() { }
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    //returns point of intersection else valid = false
    SmartPoint get_intersection_SmartPoint(Ray ray);
};


class BeautifulPlane : public Figure, public Object {
    Vec3 normal = Vec3(0, 0, 1);
    double d = 0;
    Color color = Color(255, 255, 255);
    Material material = Material("dielectric");
public:
    BeautifulPlane() { }
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    SmartPoint get_intersection_SmartPoint(Ray ray);
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
    Vec3 position = Vec3(0, 0, 0);
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
    Vec3 direction = Vec3(0, 0, 0);;
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
    double FOV_X, FOV_Y; // ширина поля зрения вдоль WIDTH: 1,57 - 90 Градусов
    int X, Y; // ширина и высота экрана
    bool i_am_fish = 0;
    Matr3 calculate_rotate();
    Ray calculate_ray(unsigned x, unsigned y);
public:
    int height() const;
    int width() const;
    Object* clone(std::vector<std::string> const & arg);
    std::pair<int,std::string> name() const;
    Camera(); // пустой конструктор (источник в начале координат, углы нулевые, расстояние, ширина, высота единицы)
    Camera(Vec3 position, Vec3 angels, double FOV_X, int X, int Y, bool i_am_fish = 0);
    std::vector<Ray> create_rays(); // генерация лучей
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

    const Camera* get_camera() const;
    Color** get_res();

    // fills scene with objects
    void initialization(const char * input);

    // calculates everything
    void render();
    
    // inner methods
    void trace_ray(Ray ray);
    double full_intensity_in_point(Vec3 point, Vec3 normal);
    SmartPoint get_first_SmartPoint(Ray ray);
};
