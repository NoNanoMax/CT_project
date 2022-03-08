/*

    scene.cpp

Программный файл для scene.h

*/

#include <vector>
#include "math.h"
#include "scene.h"

// ------------------------------- camera -------------------------------

Matr3 Camera::calculate_rotate() {
    Matr3 a = Matr3(cos(angles[0]), -sin(angles[0]),    0,
                    sin(angles[0]), cos(angles[0]),     0,
                    0,              0,                  1);
    Matr3 b = Matr3(1, 0,              0,
                    0, cos(angles[1]), -sin(angles[1]),
                    0, sin(angles[1]), cos(angles[1]));
    Matr3 c = Matr3(cos(angles[2]), -sin(angles[2]),    0,
                    sin(angles[2]), cos(angles[2]),     0,
                    0,              0,                  1);
    return a * b * c;
}

Ray Camera::calculate_ray(unsigned x, unsigned y) {
    Vec3 screen_point(width * x / width_pixels - width / 2, distance, height  * y / height_pixels - height / 2);
    Vec3 direction = (rotate * (screen_point - position)).normalized();
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
    width_pixels = 1280, height_pixels = 720;
}

std::vector<std::vector<Ray> > Camera::create_rays() {
    std::vector<std::vector<Ray> > ret;
    for(unsigned x = 0; x < width_pixels; x++) {
        std::vector<Ray> v;
        for(unsigned y = 0; y < height_pixels; y++) {
            v.push_back(Ray(calculate_ray(x, y)));   
        }
        ret.push_back(v);
    }
    return ret;
}

// ------------------------------- scene -------------------------------

