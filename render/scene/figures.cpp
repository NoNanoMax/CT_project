/*

    figures.cpp

Программный файл для figures.h

*/

#include "scene.h"
#include "math.h"
#include <vector>

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n1, Vec3 n2, Vec3 n3): v1(v1), v2(v2), v3(v3), n1(n1), n2(n2), n3(n3) { }

bool Triangle::check_intersection(Ray r) {
    if (2 == 2) {
        return true;
    }
    return false;
}

Sphere::Sphere(Vec3 center, double radius, unsigned iterations) {
    body.push_back(Triangle(center + Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)));
    body.push_back(Triangle(center + Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, -1)));
    body.push_back(Triangle(center + Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, 1)));
    body.push_back(Triangle(center + Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1)));
    body.push_back(Triangle(center - Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)));
    body.push_back(Triangle(center - Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, -1)));
    body.push_back(Triangle(center - Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, 1)));
    body.push_back(Triangle(center - Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1)));
    std::vector<Triangle> next;
    for(unsigned j = 0; j < iterations; j++) {
        unsigned amount = body.size();
        for(unsigned i = 0; i < amount; i++) {
            Vec3 v1 = ((body[i].v1 + body[i].v2) / 2 - center).normalized() * radius + center;
            Vec3 v2 = ((body[i].v2 + body[i].v3) / 2 - center).normalized() * radius + center;
            Vec3 v3 = ((body[i].v1 + body[i].v3) / 2 - center).normalized() * radius + center;
            next.push_back(Triangle(v1, v2, v3, (v1 - center).normalized(), (v2 - center).normalized(), (v2 - center).normalized()));
            next.push_back(Triangle(body[i].v1, v1, v3, (body[i].v1 - center).normalized(), (v1 - center).normalized(), (v3 - center).normalized()));
            next.push_back(Triangle(body[i].v2, v1, v2, (body[i].v2 - center).normalized(), (v1 - center).normalized(), (v2 - center).normalized()));
            next.push_back(Triangle(body[i].v3, v2, v3, (body[i].v3 - center).normalized(), (v2 - center).normalized(), (v3 - center).normalized()));
        }
        body = next;
        next.clear();
    }
}

bool Sphere::check_intersection(Ray r) {
    return true;
}

std::vector<Triangle> Sphere::get_body() {
    return body;
}

