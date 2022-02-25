/*
    main.cpp

Ну main

*/

#include "render/render.h"
#include "stdio.h"
#include <vector>

int main() {
    Vec3 v(0, 0, 0);
    Sphere S(v, 10);
    std::vector<Triangle> t = S.get_body();
    for (int i = 0; i < t.size(); i++) {
        printf("(%f, %f, %f)\n", t[i].v1.x, t[i].v1.y, t[i].v1.z);
    }
    return 0;
}
