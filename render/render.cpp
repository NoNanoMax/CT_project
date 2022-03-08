/*

    render.cpp

Программный файл для render.cpp

*/

#include "render.h"

Renderer::Renderer() {
    Sphere s(Vec3(0, 5, 0), 1);
    scene.figures.push_back(&s);

    Point_light l(Vec3(5, 2, 2), 1);
    scene.lights.push_back(&l);
}
