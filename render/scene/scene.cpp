/*

    scene.cpp

Программный файл для работы сцены

*/

#include <vector>
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

std::vector<Figure*> Scene::get_figures() {
    return figures;
}

void Scene::res_clear() {
    for (short i = 0; i < WIDTH; i++) {
        for (short j = 0; j < HEIGHT; j++) {
            //res[i][j].make(std::string("black"));
            res[i][j].r = 0, res[i][j].g = 0, res[i][j].b = 0;
        }
    }
}

void Scene::figures_clear() {
    figures.clear();
}

void Scene::lights_clear() {
    lights.clear();
}

void Scene::figures_add(Figure* figure) {
    figures.push_back(figure);
}

void Scene::lights_add(Light_source* light) {
    lights.add(light);
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

Scene::Scene(std::vector<Figure*> figures, Lights_manager lights, Camera camera):
    figures(figures), lights(lights), camera(camera) {
}

void Scene::trace_ray(Ray ray) {
    Triangle intersected; // треугольник пересечения
    double min = 1e5; // минимум расстояния до пересеченного треугольника
    for (std::vector<Figure*>::iterator it = figures.begin(); it != figures.end(); it++) {
        Triangle tr = (*it)->get_intersection_triangle(ray); // получаем треугольник от фигуры
        if (!tr.is_empty() && (tr.get_intersection_point(ray) - ray.from).abs() < min) {
            min = (tr.get_intersection_point(ray) - ray.from).abs();
            intersected = tr;
        }
    }
    if (!intersected.is_empty()) {
        Vec3 point = intersected.get_intersection_point(ray);
        Vec3 normal = intersected.get_normal(point);
        res[ray.x][ray.y].r += short(lights.full_intensity_in_point(point, normal) * 255);
    }
}

void Scene::render() {
    std::vector<Ray> rays = camera.create_rays();
    for (std::vector<Ray>::iterator it = rays.begin(); it != rays.end(); it++) {
        trace_ray(*it);
    }
}



