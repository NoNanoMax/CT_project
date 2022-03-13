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

Ambient_light::Ambient_light() {}

std::pair<int, std::string> Ambient_light::name() const{
    return std::pair<int, std::string>(LIGHT, "Ambient_light");
}

Object* Ambient_light::clone(std::vector<std::string> const &arg){
    assert(arg.size() >= 1);
    Ambient_light* rez = new Ambient_light();
    rez->intensity =  atof(arg[0].c_str());
    return rez;
}

void Ambient_light::build(double intensity) {
    this->intensity = intensity;
}

double Ambient_light::intensity_in_point(Vec3 point, Vec3 normal) {
    return intensity;
};

// ------------------------------- Point_light -------------------------------

Point_light::Point_light() {}

std::pair<int, std::string> Point_light::name() const{
    return std::pair<int, std::string>(LIGHT,"Point_light");
}

Object* Point_light::clone(std::vector<std::string> const &arg){
    assert(arg.size() >= 4);
    Point_light* rez = new Point_light();
    rez->position = Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    rez->intensity =  atof(arg[3].c_str());
    return rez;
}

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

std::pair<int, std::string> Directed_light::name() const{
    return std::pair<int, std::string>(LIGHT,"Directed_light");
}

Object* Directed_light::clone(std::vector<std::string> const &arg){
    assert(arg.size() >= 4);
    Directed_light* rez = new Directed_light();
    rez->direction = Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    rez->intensity =  atof(arg[3].c_str());
    return rez;
}

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