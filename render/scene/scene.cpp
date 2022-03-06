/*

    scene.cpp

Программный файл для scene.h

*/

#include <vector>
#include "lights.cpp"
#include "scene.h"

class Figure{

};



class Camera{
    Vec3 position;
    Matr3 rotate;
    Vec3 angles; // psi, theta, pfi;
    Vec3 size; // height, width_h, width_v; высота камеры горизонтальная ширина вертикальная ширина
    int X, Y;

Matr3 get_rotate(const Vec3& ang){
    Matr3 a = Matr3(Vec3(cos(angles[0]), sin(angles[0]), 0),
                    Vec3(-sin(angles[0]), cos(angles[0]), 0),
                    Vec3(0,0, 1)
    );
    Matr3 b = Matr3(Vec3(1, 0, 0),
                    Vec3(0, cos(angles[1]), sin(angles[1])),
                    Vec3(0, -sin(angles[1]), cos(angles[1]))
                    
    );
    Matr3 c = Matr3(Vec3(cos(angles[2]), sin(angles[2]), 0),
                    Vec3(-sin(angles[2]), cos(angles[2]), 0),
                    Vec3(0,0, 1)
    );
    return a*b*c;
}

Vec3 get_vect(int x, int y){
    return rotate*Vec3(x, y, size[0]); 
}


public:
    Camera(Vec3 pos, Vec3 ang, Vec3 size, int X, int Y):
        position(pos), angles(ang), size(size), X(X), Y(Y){
            if(size.any_null() or X <=0, Y <=0){
                printf("camera parameters error\n");
            }
            size = size*(1/size.abs());
            rotate = get_rotate(angles);
        }
    
    void compute(std::vector<Ray>& vect){
        double x_, y_;

        for(int x = -X/2, x_ = (-1/2)*size[1]; x < X; ++x, x_ += size[1]/ X){
            for(int y = -Y/2, y_ = (-1/2)*size[2]; y < Y/2; ++y, y_ += size[2]/ X){
                Vec3 point = get_vect(x_,y_);
                vect.push_back(Ray(position, point, 1, INFINITY,x,y));       
            }
        }
    }
};



class Scene{
    std::vector<Figure*> figures;
    Light light;


/*
    Vec3 trace_ray(const Scene& const Sc, const Light const Li, 
                Vec3 begin, Vec3 vec, double min_t,double max_t) const{
    Object A;
    Vec3 point;
    Sc.intersect(begin, vec, min_t, max_t, &A, &point);
    double light = Li.get_light(Sc, begin, point, A.normal(point), A.alpha());

}*/
};
