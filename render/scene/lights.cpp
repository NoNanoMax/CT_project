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
#include "CT_project/render/scene/linal/linal.cpp"
//using namespace std;

typedef Vec3 vector;
typedef Matr3 matrix;

class Light_source{
public:
    
    Light_source(){}
    virtual double get_intense(vector camera, vector normal, int alpha) = 0;
    virtual double intense() = 0;
protected:  
    inline double get_intence_in(const vector camera, const vector normal, const vector& direction, int alpha){
    if(normal.is_null()) return 0;
    if(direction.is_null()) return 0;
    
    double in = -(direction * normal);
    if(in <= 0) return 0;
    if(alpha < 0){
        
        return in/(direction.abs() * normal.abs());
    }
    if( camera.is_null()) return 0;
    vector R = 2*((direction*normal)/(normal*normal))*normal + direction;
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
    double get_intense(vector camera, vector normal, int alpha){
        return intens;
    }
};

class Point_light: public Light_source{
protected:    
    vector pos;
    double intens;
public:
    Point_light(vector pos, double inte): pos(pos), intens(inte){}
    double intense(){
        return intens;
    }
    double get_intense(vector camera, vector normal, int alpha){
        if(normal.is_null()) return 0;
        vector direction = camera - pos;
        return intens * get_intence_in(camera, normal, direction, alpha) / direction.abs();
    }
};

class Directed_liht: public Light_source{
protected:
    vector direction;
    double intens;

public:
    Directed_liht(vector dir, double inte): direction(dir), intens(inte){
            }

    double intense(){
        return intens;
    }

    double get_intense(vector camera, vector normal, int alpha){
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

    double get_light(vector camera, vector point, vector normal, int alpha = -1){
        camera = camera - point;
        double rez = 0;
        for(auto ll: light){
            rez += ll->get_intense(camera, normal, alpha);
        }
        return rez/sum_intense;
    }


};


class Camera{
    vector position;
    matrix rotate;
    vector angles; // psi, theta, pfi;
    vector size; // height, width_h, width_v; высота камеры горизонтальная ширина вертикальная ширина
    int X, Y;

matrix get_rotate(const vector& ang){
    matrix a = matrix(vector(cos(angles[0]), sin(angles[0]), 0),
                    vector(-sin(angles[0]), cos(angles[0]), 0),
                    vector(0,0, 1)
    );
    matrix b = matrix(vector(1, 0, 0),
                    vector(0, cos(angles[1]), sin(angles[1])),
                    vector(0, -sin(angles[1]), cos(angles[1]))
                    
    );
    matrix c = matrix(vector(cos(angles[2]), sin(angles[2]), 0),
                    vector(-sin(angles[2]), cos(angles[2]), 0),
                    vector(0,0, 1)
    );
    return a*b*c;
}

vector get_vect(int x, int y){
    return rotate*vector(x, y, size[0]); 
}

vector trace_ray(const Scene& const Sc, const Light const Li, 
                vector begin, vector vec, double min_t,double max_t) const{
    Object A;
    vector point;
    Sc.intersect(begin, vec, min_t, max_t, &A, &point);
    double light = Li.get_light(Sc, begin, point, A.normal(point), A.alpha());

}


public:
    Camera(vector pos, vector ang, vector size, int X, int Y):
        position(pos), angles(ang), size(size), X(X), Y(Y){
            if(size.any_null() or X <=0, Y <=0){
                printf("camera parameters error\n");
            }
            size = size*(1/size.abs());
            rotate = get_rotate(angles);
        }
    
    void compute(const Scene& const Sc,const Light const Li){
        double x_, y_;
        for(int x = -X/2, x_ = (-1/2)*size[1]; x < X; ++x, x_ += size[1]/ X){
            for(int y = -Y/2, y_ = (-1/2)*size[2]; y < Y/2; ++y, y_ += size[2]/ X){
                vector point = get_vect(x_,y_);
                point = trace_ray(Sc, Li, position, point, 1, INFINITY);
                paint(x,y, point);
            }
        }
    }

};
