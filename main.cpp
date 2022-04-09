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

    double max_light = 0;
	for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            
            max_light += int(res[i][j].r) + int(res[i][j].g) + int(res[i][j].b);
        }
   	}
    max_light /= height*width;
    
    max_light = 1;
   	for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            printf("%f %f %f ", 1.0*res[i][j].r/max_light, 1.0*res[i][j].g/max_light, 1.0*res[i][j].b/max_light);
        }
        printf("\n");
   	}
	printf("\n");

    return 0;
}
