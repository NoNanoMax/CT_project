/*

    lights.cpp

Программный файл для lights.h

освещение, в котором три класса свата наследуются от одного класса свет

*/

#include <vector>
#include "math.h"
#include "scene.h"

class Light {
public:
    std::vector<Light_source*> sources;
    Light() { }
    double get_light(Vec3 point, Vec3 normal) {
        camera = camera - point;
        double rez = 0;
        for(auto ll: light) {
            rez += ll->get_intense(camera, normal, alpha);
        }
        return rez / sum_intense;
    }
};

class Light_source {
public:
    virtual double get_intense(Vec3 camera, Vec3 normal, int alpha) = 0;
    virtual double intense() = 0;
protected:  
    inline double get_intence_in_point(const Vec3 point, const Vec3 norm, const Vec3& direction, int alpha) {
        double inte = - dot(direction, normal);
        if (inte <= 0) return 0;
        if (alpha < 0) {
            return inte / direction.abs();
        }
        if (camera.is_null()) return 0;
        Vec3 R = 2 * dot(direction, normal) * normal + direction;
        in = R * camera;
        if(in <= 0) return 0;
        return pow(in / (R.abs() * camera.abs()), alpha);    
    }
};

class Ambient_light: public Light_source {
protected:
    double intens;
public: 
    Ambient_light(double inte): intens(inte) {}
    double intense() {
        return intens;
    }    
    double get_intense(Vec3 camera, Vec3 normal, int alpha) {
        return intens;
    }
};

class Point_light: public Light_source {
protected:    
    Vec3 pos;
    double intens;
public:
    Point_light(Vec3 pos, double inte): pos(pos), intens(inte) {}
    double intense() {
        return intens;
    }
    double get_intense(Vec3 camera, Vec3 normal, int alpha) {
        if(normal.is_null()) return 0;
        Vec3 direction = camera - pos;
        return intens * get_intence_in(camera, normal, direction, alpha) / direction.abs();
    }
};

class Directed_liht: public Light_source {
protected:
    Vec3 direction;
    double intens;
public:
    Directed_liht(Vec3 dir, double inte): direction(dir), intens(inte) {}
    double intense() {
        return intens;
    }
    double get_intense(Vec3 camera, Vec3 normal, int alpha){
        return intens * get_intence_in(camera, normal, direction, alpha);
    }
};
