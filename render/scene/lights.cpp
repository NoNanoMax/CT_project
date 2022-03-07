/*

    lights.cpp

Программный файл для lights.h

освещение, в котором три класса свата наследуются от одного класса свет

*/

#include <vector>
#include <string>
#include "math.h"
#include "scene.h"

Lights::Lights() { }

void Lights::push_back(Light_source* source) {
    sources.push_back(source);
}

double Lights::full_intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = 0;
    for(std::vector<Light_source*>::iterator it = sources.begin(); it != sources.end(); it++) {
        ret += (*it)->intensity_in_point(point, normal);
    }
    return ret;
}

Ambient_light::Ambient_light(double intensity): intensity(intensity) { }

double Ambient_light::intensity_in_point(Vec3 point, Vec3 normal) {
    return intensity;
};

Point_light::Point_light(Vec3 position, double intensity): position(position), intensity(intensity) { }

double Point_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(point - position, normal) * intensity;
    return (ret > 0 ? ret : 0);
}


Directed_light::Directed_light(Vec3 direction, double intensity): direction(direction), intensity(intensity) { }

double Directed_light::intensity_in_point(Vec3 point, Vec3 normal) {
    double ret = - cos(direction, normal) * intensity;
    return (ret > 0 ? ret : 0);
}
