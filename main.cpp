/*
    main.cpp

Ну main

*/


#include "render/scene.h"
#include "iostream"
#include <vector>

int main() {
    Scene r;
    r.initialization("input.txt");
    r.render();
    Color** res = r.get_res();
	
    printf("%d %d %d\n", HEIGHT, WIDTH, 3);
   	for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            printf("%03d %03d %03d ", res[i][j].r, res[i][j].g, res[i][j].b);
        }
   	}
	std::cout << "\n";

    return 0;
}
