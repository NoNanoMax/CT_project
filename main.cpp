/*
    main.cpp

Ну main

*/

#include "render/render.h"
#include "stdio.h"

int main() {
    Vec3 v(1, 1, 1), u(1, 2, 3);
    Vec3 a = v - u;
    double d = dot(v, u);
    printf("(%lf, %lf, %lf)\n %f", a.x, a.y, a.z, d);
    return 0;
}
