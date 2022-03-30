/*

    lights.cpp

Программный файл для описния сета

освещение, в котором три класса свата наследуются от одного класса свет

*/

#include <vector>
#include <string>
#include "math.h"
#include "scene.h"


SmartPoint get_first_SmartPoint(Ray r, std::vector<Figure*>* figures_pointer) {
    SmartPoint intersected(false); // точка пересечения

    double min = 1e5; // минимум расстояния до пересеченной фигуры
    for (std::vector<Figure*>::iterator it = figures_pointer->begin(); it != figures_pointer->end(); it++) {
        SmartPoint point = (*it)->get_intersection_SmartPoint(r);
        if (point.valid && (point.point - r.pos).abs() < min) {
            min = (point.point - r.pos).abs();
            intersected = point;
        }
    }
    //if no intersection returns "negative" result
    return intersected;
}

// ------------------------------- Ambient_light -------------------------------

Ambient_light::Ambient_light() { }

std::pair<int, std::string> Ambient_light::name() const{
    return std::pair<int, std::string>(LIGHT, "Ambient_light");
}

Object* Ambient_light::clone(std::vector<std::string> const &arg){
    assert(arg.size() >= 1);
    Ambient_light* rez = new Ambient_light();
    rez->intensity =  atof(arg[0].c_str());
    return rez;
}

double Ambient_light::intensity_in_point(SmartPoint smartpoint, std::vector<Figure*>* figures_pointer) {
    return intensity;
};

// ------------------------------- Point_light -------------------------------

Point_light::Point_light() { }

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

double Point_light::intensity_in_point(SmartPoint smartpoint, std::vector<Figure*>* figures_pointer) {
    Ray target_ray = Ray(smartpoint.point, (position - smartpoint.point).normalized());
    target_ray.pos = target_ray.pos + smartpoint.normal * ZERO;
    SmartPoint res = get_first_SmartPoint(target_ray, figures_pointer);

    if (!res.valid || ((position - smartpoint.point).abs() < (res.point - smartpoint.point).abs())) {
        double ret = - cos(smartpoint.point - position, smartpoint.normal) * intensity;
        return (ret > 0 ? ret : 0);
    }
    return 0;
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

double Directed_light::intensity_in_point(SmartPoint smartpoint, std::vector<Figure*>* figures_pointer) {
    Ray target_ray = Ray(smartpoint.point, Vec3(0, 0, 0) - direction);
    SmartPoint res = get_first_SmartPoint(target_ray, figures_pointer);
    if (!res.valid) {
        double ret = - cos(direction, smartpoint.normal) * intensity;
        return (ret > 0 ? ret : 0);
    }
    return 0;
}
