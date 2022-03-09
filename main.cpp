/*
    main.cpp

Ну main

*/


#include "render/render.h"
#include "iostream"
#include <vector>

int main() {
    Renderer r;
    r.read();
    r.render();
    Color** res = r.res;

    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            printf("%03d ", res[i][j].r);
        }
        std::cout << std::endl;
    }

    return 0;
}
