/*
    main.cpp

Ну main

*/


#include "render/scene.h"
#include "stdio.h"
#include <vector>

int main() {
    Scene r;
    r.initialization("tmp_info.ass");
    r.render();

    Color** res = r.get_res();
    int height =  r.get_camera()->height();
    int width = r.get_camera()->width();
    printf("%d %d %d\n", height, width, 3);

   	for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            printf("%f %f %f ", 1.0*res[i][j].r, 1.0*res[i][j].g, 1.0*res[i][j].b);
        }
        printf("\n");
   	}
	printf("\n");

    return 0;
}
