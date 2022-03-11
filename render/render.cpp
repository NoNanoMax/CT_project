/*

    render.cpp

Программный файл для render.cpp

*/

#include "render.h"

Renderer::Renderer() { }


#include <iostream>
void Renderer::read() {
    Sphere* ps = new Sphere;
    ps->build(Vec3(0, 5, 0), 1);

    Point_light* pl = new Point_light;
    pl->build(Vec3(0, 3, 0), 1);

    Ambient_light* al = new Ambient_light(0.3);

    scene.lights_add(pl);
    scene.figures_add(ps);
    scene.lights_add(al);
}

void Renderer::render() {
    scene.render();
    res = scene.get_res();
}
