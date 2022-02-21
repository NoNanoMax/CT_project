/*
    main.cpp

Ну main

*/

#include "render/render.h"
#include "stdio.h"

int main() {
    Vec3 v(1.0, 1.0, 1.0), u(2., 2., 2.);
    Ray r(v, v);
    
    Triangle t(v, u, v);
    printf("%d\n", t.check_intersection(r));
    return 0;
}
