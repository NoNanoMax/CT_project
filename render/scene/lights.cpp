/*

    lights.cpp

Программный файл для lights.h

*/

#include "scene.h"

// камера и освещение
// здесь представлен класс камеры и методы отрисовки камеры
// а также освещение, в котором три класса свата наследуются от одного класса свет
// 

#include <vector>
#include <math.h>
#include <stdio.h>
#include "linal/linal.h"
#include "light.h"


class Light_source{
public:
    
    Light_source(){}
    virtual double get_intense(Vec3 camera, Vec3 normal, int alpha) = 0;
    virtual double intense() = 0;
protected:  
    inline double get_intence_in(const Vec3 camera, const Vec3 normal, const Vec3& direction, int alpha){
    if(normal.is_null()) return 0;
    if(direction.is_null()) return 0;
    
    double in = -(direction * normal);
    if(in <= 0) return 0;
    if(alpha < 0){
        
        return in/(direction.abs() * normal.abs());
    }
    if( camera.is_null()) return 0;
    Vec3 R = 2*((direction*normal)/(normal*normal))*normal + direction;
    in = R*camera;
    if(in <= 0) return 0;
    return pow(in/(R.abs()*camera.abs()),alpha);    
    }
};

class Ambient_light: public Light_source{
protected:
    double intens;

public: 

    Ambient_light(double inte): intens(inte){}
    double intense(){
        return intens;
    }    
    double get_intense(Vec3 camera, Vec3 normal, int alpha){
        return intens;
    }
};

class Point_light: public Light_source{
protected:    
    Vec3 pos;
    double intens;
public:
    Point_light(Vec3 pos, double inte): pos(pos), intens(inte){}
    double intense(){
        return intens;
    }
    double get_intense(Vec3 camera, Vec3 normal, int alpha){
        if(normal.is_null()) return 0;
        Vec3 direction = camera - pos;
        return intens * get_intence_in(camera, normal, direction, alpha) / direction.abs();
    }
};

class Directed_liht: public Light_source{
protected:
    Vec3 direction;
    double intens;

public:
    Directed_liht(Vec3 dir, double inte): direction(dir), intens(inte){
            }

    double intense(){
        return intens;
    }

    double get_intense(Vec3 camera, Vec3 normal, int alpha){
        return intens * get_intence_in(camera, normal, direction, alpha);
    }
};

class Light{
    std::vector<Light_source*> light;
    double sum_intense;
public:
    Light(){
        //Light_source.resize(0);
        sum_intense = 0;
    }

   /* Light(int n){
        Light_source.resize(n);
        sum_intense = 0;
    }*/

    ~Light(){
        for(auto ll: light){
            delete ll;
        }
    }

    bool push_back(Light_source* source){
        light.push_back(source);
        sum_intense += source->intense();
    }

    double get_light(Vec3 camera, Vec3 point, Vec3 normal, int alpha = -1){
        camera = camera - point;
        double rez = 0;
        for(auto ll: light){
            rez += ll->get_intense(camera, normal, alpha);
        }
        return rez/sum_intense;
    }

};
