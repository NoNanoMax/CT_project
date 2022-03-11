/*

    lights.cpp

Программный файл для описния сета

освещение, в котором три класса свата наследуются от одного класса свет

*/

#include <vector>
#include <string>
#include "math.h"
#include "scene.h"

// Lights_manager::Lights_manager() { }

// void Lights_manager::add(Light_source* source) {
//     sources.push_back(source);
// }

// void Lights_manager::clear() {
//     sources.clear();
// }

// double Lights_manager::full_intensity_in_point(Vec3 point, Vec3 normal) {
//     double ret = 0;
//     for(std::vector<Light_source*>::iterator it = sources.begin(); it != sources.end(); it++) {
//         ret += (*it)->intensity_in_point(point, normal);
//     }
//     return ret;
//}

Ambient_light::Ambient_light(double intensity):
    intensity(intensity)
{ }

double Ambient_light::intensity_in_point(Vec3 point, Vec3 normal) {
    return intensity;
};

Point_light::Point_light() { }

void Point_light::build(Vec3 position, double intensity) {
    this->position = position;
    this->intensity = intensity;
}


double Point_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(point - position, normal) * intensity;
    return (ret > 0 ? ret : 0);
}


Directed_light::Directed_light(Vec3 direction, double intensity):
    direction(direction), intensity(intensity)
{ }

double Directed_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(direction, normal) * intensity;
    return (ret > 0 ? ret : 0);
}
