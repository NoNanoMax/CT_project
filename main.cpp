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
	
    printf("%d %d %d\n", HEIGHT, WIDTH, 3);
   	for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            printf("%03d %03d %03d ", res[i][j].r, res[i][j].r, res[i][j].r);
        }
   	}
	std::cout << "\n";

    return 0;
}
