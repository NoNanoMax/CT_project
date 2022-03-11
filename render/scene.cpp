/*

    scene.cpp

Программный файл для работы сцены

*/

#include <vector>

#include <map>
#include <fstream>
#include <sstream>
#include <cassert>


#include <string>
#include "math.h"
#include "scene.h"

// ------------------------------- camera -------------------------------

Matr3 Camera::calculate_rotate() {
    Matr3 a(cos(angles[0]), -sin(angles[0]),    0,
            sin(angles[0]), cos(angles[0]),     0,
            0,              0,                  1);
    Matr3 b(1, 0,              0,
            0, cos(angles[1]), -sin(angles[1]),
            0, sin(angles[1]), cos(angles[1]));
    Matr3 c(cos(angles[2]), -sin(angles[2]),    0,
            sin(angles[2]), cos(angles[2]),     0,
            0,              0,                  1);
    return a * b * c;
}

Ray Camera::calculate_ray(unsigned x, unsigned y) {
    Vec3 screen_point(width * x / width_pixels - width / 2, distance, height  * y / height_pixels - height / 2);
    Vec3 direction(rotate * (screen_point - position).normalized());
    return Ray(position, direction, x, y, 1);
}

Camera::Camera(Vec3 position, double* angles, double distance, double width, double height, unsigned width_pixels, unsigned height_pixels):
    position(position), distance(distance), width(width), height(height), width_pixels(width_pixels), height_pixels(height_pixels) {
    for (short i = 0; i < 3; i++) this->angles[i] = angles[i];
    rotate = calculate_rotate();
}

Camera::Camera() {
    position = Vec3(0, 0, 0);
    angles[0] = 0, angles[1] = 0, angles[2] = 0;
    distance = 1, width = 1, height = 1;
    width_pixels = WIDTH, height_pixels = HEIGHT;
    rotate = calculate_rotate();
}

std::vector<Ray> Camera::create_rays() {
    std::vector<Ray> ret;
    for (unsigned x = 0; x < width_pixels; x++) {
        for (unsigned y = 0; y < height_pixels; y++) {
            ret.push_back(calculate_ray(x, y));
        }
    }
    return ret;
}

// ------------------------------- scene -------------------------------

Color** Scene::get_res() {
    return res;
}

Scene::Scene() {
    res = new Color*[WIDTH];
    for (short i = 0; i < WIDTH; i++) {
        res[i] = new Color[HEIGHT];
    }
}

Scene::~Scene() {
    for (short i = 0; i < WIDTH; i++) {
        delete[] res[i];
    }
    delete[] res;
}

Scene::Scene(std::vector<Figure*> figures, std::vector<Light_source*> lights, Camera camera):
    figures(figures), lights(lights), camera(camera) {
}


//build figures and lights from input, deafult "input.txt"
void Scene::initialization(const char * input = "input.txt") {

    //vremennoe reshenie
    BuityfulSphere* ps = new BuityfulSphere;
    ps->build(Vec3(0, 5, 0), 1);

    // Sphere* ps = new Sphere;
    // ps->build(Vec3(0, 5, 0), 1);

    Point_light* pl = new Point_light;
    pl->build(Vec3(2, 3, 3), 0.8);
    Point_light* pl1 = new Point_light;
    pl1->build(Vec3(-2, -4, -3), 0.5);

    Ambient_light* al = new Ambient_light();
    al->build(0.3);

    lights.push_back(pl);
    lights.push_back(pl1);   
    lights.push_back(al);

    figures.push_back(ps);
}

//returns normal at intesection point else vector direction equals (2, 0, 0)
inline Ray Scene::get_first_intersection(Ray ray) {
    Ray intersected(Vec3(0,0,0), Vec3(2,0,0), 0, 0, 0); // точка пересечения
    double min = 1e5; // минимум расстояния до пересеченного треугольника
    for (std::vector<Figure*>::iterator it = figures.begin(); it != figures.end(); it++) {
        Ray point = (*it)->does_intersect(ray); 
        if (!(point.dir.x == 2) && (point.from - ray.from).abs() < min) {
            min = (point.from - ray.from).abs();
            intersected = point;
        }
    }
    //if no intersection returns "negative" result
    return Ray(intersected.from, intersected.dir, 0, 0, 0);
}

double Scene::full_intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = 0;
    for(std::vector<Light_source*>::iterator it = lights.begin(); it != lights.end(); it++) {
        ret += (*it)->intensity_in_point(point, normal);
    }
    return ret;
}

void Scene::trace_ray(Ray ray) {
    // Triangle intersected; // треугольник пересечения
    // double min = 1e5; // минимум расстояния до пересеченного треугольника
    // for (std::vector<Figure*>::iterator it = figures.begin(); it != figures.end(); it++) {
    //     Triangle tr = (*it)->get_intersection_triangle(ray); // получаем треугольник от фигуры
    //     if (!tr.is_empty() && (tr.get_intersection_point(ray) - ray.from).abs() < min) {
    //         min = (tr.get_intersection_point(ray) - ray.from).abs();
    //         intersected = tr;
    //     }
    // }

    Ray intersetion_point = get_first_intersection(ray);
    if (intersetion_point.dir.x != 2) {


        res[ray.x][ray.y].r += short(full_intensity_in_point(intersetion_point.from, intersetion_point.dir) * 255)*abs(dot(intersetion_point.dir, Vec3(1,0,0)));
        res[ray.x][ray.y].g += short(full_intensity_in_point(intersetion_point.from, intersetion_point.dir) * 255)*abs(dot(intersetion_point.dir, Vec3(0,1,0)))*0.5;
        res[ray.x][ray.y].b += short(full_intensity_in_point(intersetion_point.from, intersetion_point.dir) * 255)*abs(dot(intersetion_point.dir, Vec3(0,0,1)));

        if (res[ray.x][ray.y].r > 255) res[ray.x][ray.y].r = 255;
        if (res[ray.x][ray.y].g > 255) res[ray.x][ray.y].g = 255;
        if (res[ray.x][ray.y].b > 255) res[ray.x][ray.y].b = 255;



    }
}

void Scene::render() {
    std::vector<Ray> rays = camera.create_rays();
    for (std::vector<Ray>::iterator it = rays.begin(); it != rays.end(); it++) {
        trace_ray(*it);
    }
}



