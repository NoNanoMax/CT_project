/*
    render.h

Здесь производится обработка камеры и выпуск лучей

*/

#include "scene/scene.h"

class Renderer {
    Scene scene;
public:
    Color** res;
    Renderer();
    void read();
    void render();
};


