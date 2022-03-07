/*
    main.cpp

Ну main

*/

#include "render/render.h"
#include "iostream"
#include <vector>

int main() {
    Vec3 o = Vec3(0, 0, 0);
    Vec3 x = Vec3(1, 0, 0);
    Vec3 y = Vec3(0, 1, 0);
    Vec3 z = Vec3(0, 0, 1);
    Ambient_light l1 = Ambient_light(0.25);
    Point_light l2 = Point_light(o, 1);
    Directed_light l3 = Directed_light(x, 0.5);
    Lights all;
    all.push_back(&l1);
    all.push_back(&l2);
    all.push_back(&l3);
    std::cout << all.full_intensity_in_point(2 * x,  Vec3(-1, 0, 0)) << std::endl;

    double ang[3];
    ang[0] = 0, ang[1] = 0, ang[2] = 0;
    Camera camera(o, ang, 1, 1, 1, 10, 10);

    std::vector<std::vector<Ray> > v = camera.create_rays();
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            std::cout << v[i][j].dir.z << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
