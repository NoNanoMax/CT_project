/*
    main.cpp

Ну main

*/

#include "render/render.h"
#include "stdio.h"
#include <vector>

int main() {
    Vec3 v(1, 1, 1);
    //Sphere S(v, 10);
    //std::vector<Triangle> t = S.get_body();
    //for (int i = 0; i < t.size(); i++) {
    //    printf("(%f, %f, %f)\n", t[i].v1.x, t[i].v1.y, t[i].v1.z);
    //}
    Matr3 m(3, 6, 9, 12, 15, 18, 21, 24, 27);
    Matr3 n(2, 4, 6, 8, 10, 12, 14, 16, 18);

    m.trans();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            printf("%f ", m.body[i][j]);
        }
         printf("\n");
    }

    return 0;
}
