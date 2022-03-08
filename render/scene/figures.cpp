/*

    figures.cpp

Программный файл для figures.h

*/

#include "scene.h"
#include "math.h"
#include <vector>

// ------------------------------- Triangle -------------------------------

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n1, Vec3 n2, Vec3 n3): v1(v1), v2(v2), v3(v3), n1(n1), n2(n2), n3(n3) {
    N = vv(v2 - v1, v3 - v1).normalized();
    D = - dot(v1, N);
    Vec3 s1 = v2 - v1, s2 = v3 - v1;
    U = (s1 * dot(s2, s2) - s2 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    u0 = dot(v1, U);
    V = (s2 * dot(s1, s1) - s1 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    v0 = dot(v1, V);
}

bool Triangle::check_intersection(Ray ray) {
    if (abs(dot(N, ray.dir)) < 1e-9) return false; // проверка на параллельность
    double t = - (dot(N, ray.from) + D) / dot(N, ray.dir);
    if (t < 0) return false; // смотрит в другую сторону
    Vec3 P = ray.from + ray.dir * t; // точка пересечения
    double u = dot(P, U) - u0; // барицентрические
    double v = dot(P, V) - v0; // координаты
    if (0 <= u <= 1 && 0 <= v <= 1 && u + v > 1) {
        return true;
    } else {
        return false;
    }
}

Vec3 Triangle::get_intersection_point(Ray ray) {
    double t = - (dot(N, ray.from) + D) / dot(N, ray.dir);
    return ray.from + ray.dir * t;
}

Vec3 Triangle::get_normal(Vec3 P) {
    double u = dot(P, U) - u0;
    double v = dot(P, V) - v0;
    double w = 1 - u - v;
    return (n2 * u + n3 * v + n1 * w).normalized();
}

// ------------------------------- Sphere -------------------------------

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
    for (unsigned j = 0; j < iterations; j++) {
        unsigned amount = body.size();
        for (unsigned i = 0; i < amount; i++) {
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

Triangle* Sphere::get_intersection_triangle(Ray ray) {
    Triangle* ret = NULL;
    double min = 1e10; // ищем ближайшее пересечение
    for (std::vector<Triangle>::iterator it = body.begin(); it != body.end(); it++) {
        if (it->check_intersection(ray)) {
            if ((it->get_intersection_point(ray) - ray.from).abs() < min) {
                min = (it->get_intersection_point(ray) - ray.from).abs();
                ret = &(*it);
            }
        }
    }
    return ret;
}

std::vector<Triangle> Sphere::get_body() {
    return body;
}

