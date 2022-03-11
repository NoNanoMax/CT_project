/*

    lights.cpp

Программный файл для описния сета

освещение, в котором три класса свата наследуются от одного класса свет

*/

#include <vector>
#include <string>
#include "math.h"
#include "scene.h"

// ------------------------------- Ambient_light -------------------------------

Ambient_light::Ambient_light() { }

void Ambient_light::build(double intensity) {
    this->intensity = intensity;
}

double Ambient_light::intensity_in_point(Vec3 point, Vec3 normal) {
    return intensity;
};

// ------------------------------- Point_light -------------------------------

Point_light::Point_light() { }

void Point_light::build(Vec3 position, double intensity) {
    this->position = position;
    this->intensity = intensity;
}

double Point_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(point - position, normal) * intensity;
    return (ret > 0 ? ret : 0);
}

// ------------------------------- Directed_light -------------------------------

Directed_light::Directed_light() { }

void Directed_light::build(Vec3 direction, double intensity){
    this->direction = direction;
    this->intensity = intensity;
}

double Directed_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(direction, normal) * intensity;
    return (ret > 0 ? ret : 0);
}

Vec3 Point_light::get_position(Vec3 point) {
    return this->position;
}

Vec3 Directed_light::get_position(Vec3 point) {
    return point - this->direction;
}

Vec3 Ambient_light::get_position(Vec3 point) {
    return point;
}