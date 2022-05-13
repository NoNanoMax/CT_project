/*
    main.cpp

Ну main

*/


#include "render/scene.h"
#include "stdio.h"
#include <errno.h>
#include <string.h>
#include <vector>
#include <stdexcept>

int main() {
    Scene r;
    try{
        r.initialization("tmp_info.ass");
    } catch(const char* err_st){
        fprintf(stderr, "ошибка при чтении сцены:\n     %s\n", err_st);
        return 0;
    } catch(...){
        fprintf(stderr, "ошибка при чтении сцены:\n     %s\n", strerror(errno));

        return 0;
    }
    try{
        r.render();
    } catch(const char* err_st){
        fprintf(stderr, "ошибка при отрисовке сцены: %s\n", err_st);
        return 0;
    } catch(...){
        fprintf(stderr, "ошибка при отрисовке сцены:\n    %s\n", strerror(errno));
        return 0;
    }
    
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
