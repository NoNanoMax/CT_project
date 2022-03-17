/*

    figures.cpp

Программный файл для фигур

*/

#include "scene.h"
#include "math.h"
#include <vector>

// ------------------------------- Triangle -------------------------------
/*
Triangle::Triangle() {
    Vec3 v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0);
    Vec3 n1(0, 0, 0), n2(0, 0, 0), n3(0, 0, 0);
    Vec3 N(0, 0, 0), U(0, 0, 0), V(0, 0, 0);
    double u0 = 0, v0 = 0;
}

bool Triangle::is_empty() {
    if (n1.x == 0 && n1.y == 0 && n1.z == 0) {
        return true;
    }
    return false;
}

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n1, Vec3 n2, Vec3 n3):
    v1(v1), v2(v2), v3(v3), n1(n1), n2(n2), n3(n3) {
    N = vv(v2 - v1, v3 - v1).normalized();
    D = - dot(v1, N);
    Vec3 s1 = v2 - v1, s2 = v3 - v1;
    U = (s1 * dot(s2, s2) - s2 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    u0 = dot(v1, U);
    V = (s2 * dot(s1, s1) - s1 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    v0 = dot(v1, V);
}

bool Triangle::check_intersection(Ray ray) {
    if (abs(dot(N, ray.dir)) < 1e-10) return false; // проверка на параллельность
    double t = - (dot(N, ray.pos) + D) / dot(N, ray.dir);
    if (t < 0) return false; // смотрит в другую сторону
    Vec3 P = ray.pos + ray.dir * t; // точка пересечения
    double u = dot(P, U) - u0; // барицентрические
    double v = dot(P, V) - v0; // координаты
    if (0 <= u && u <= 1 && 0 <= v && v <= 1 && u + v <= 1) {
        return true;
    } else {
        return false;
    }
}

Vec3 Triangle::get_intersection_point(Ray ray) {
    double t = - (dot(N, ray.pos) + D) / dot(N, ray.dir);
    return ray.pos + ray.dir * t;
}

Vec3 Triangle::get_normal(Vec3 P) {
    double u = dot(P, U) - u0;
    double v = dot(P, V) - v0;
    double w = 1 - u - v;
    return (n2 * u + n3 * v + n1 * w).normalized();
}
*/
// ------------------------------- PolygonizedFigures -------------------------------

//Sphere::Sphere() { }
/*
void Sphere::build(Vec3 center, double radius, unsigned iterations) {
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

Triangle Sphere::get_intersection_triangle(Ray ray) {
    double min = 1e10; // ищем ближайшее пересечение
    Triangle ret;
    for (std::vector<Triangle>::iterator it = body.begin(); it != body.end(); it++) {
        if (it->check_intersection(ray)) {
            if ((it->get_intersection_point(ray) - ray.pos).abs() < min) {
                min = (it->get_intersection_point(ray) - ray.pos).abs();
                ret = *it;
            }
        }
    }
    return ret;
}

Ray Sphere::does_intersect(Ray ray) {
    Triangle intersected; // треугольник пересечения
       
    Triangle tr = get_intersection_triangle(ray); // получаем треугольник от фигуры
    if (!tr.is_empty())
        return Ray(tr.get_intersection_point(ray), tr.get_normal(tr.get_intersection_point(ray)));
    else 
        return Ray(Vec3(0,0,0), Vec3(2,0,0));
}

std::pair<int,std::string> Sphere::name() const{
    return std::pair<int,std::string>(FIGURE, "Sphere");
}
*/

// ------------------------------- AnaliticFigures -------------------------------

SmartPoint::SmartPoint(Vec3 point, Vec3 normal):
    point(point), normal(normal)
{ }

SmartPoint::SmartPoint(Vec3 point, Vec3 normal, Color color, Material material):
    point(point), normal(normal), color(color), material(material)
{ }

// ------------------------------- BeautifulSphere -------------------------------

Object* BeautifulSphere::clone(std::vector<std::string> const &arg) {
    assert(arg.size() >= 4);
    BeautifulSphere* rez = new BeautifulSphere();
    rez->position =  Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    rez->radius = atof(arg[3].c_str());
    if (arg.size() >= 5) rez->color = Color(arg[4]);
    return rez;
}

std::pair<int,std::string> BeautifulSphere::name() const{
    return std::pair<int,std::string>(FIGURE, "BeautifulSphere");
}

Vec3 BeautifulSphere::get_intersection_point(Ray ray) {
    double b = 2 * dot(ray.dir, ray.pos - position);
    double c = dot(ray.pos - position, ray.pos - position) - radius * radius;
    double delta = b * b - 4 * c; // детерминант
   if (delta > 0) {
       double t1 = (- b + sqrt(delta)) / 2;
       double t2 = (- b - sqrt(delta)) / 2;
       if (t1 > 0 and t2 > 0) {
           if (t1 > t2) t1 = t2;
           return ray.pos + t1 * ray.dir;
       }
   }
   return Vec3(0, 0, 0, false);
}

SmartPoint BeautifulSphere::get_intersection_SmartPoint(Ray ray) {
    Vec3 point = get_intersection_point(ray);
    Vec3 normal = point - this->position;
    return SmartPoint(point, normal, color, material);
}
  
// ------------------------------- BeautifulPlane -------------------------------

std::pair<int,std::string> BeautifulPlane::name() const {
    return std::pair<int,std::string>(FIGURE, "BeautifulPlane");
}

Object* BeautifulPlane::clone(std::vector<std::string> const &arg) {
    assert(arg.size() >= 4);
    BeautifulPlane* rez = new BeautifulPlane();
    rez->normal =  Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str())).normalized();
    rez->d = atof(arg[3].c_str());
    if (arg.size() >= 5) rez->color = Color(arg[4]);
    return rez;
}

Vec3 BeautifulPlane::get_intersection_point(Ray ray) {
    double k = dot(normal, ray.dir);
    if (k == 0) return Vec3(0, 0, 0, false); // параллельность
    double t = (this->d - dot(ray.pos, normal)) / k;

    if (t < 0) return Vec3(0, 0, 0, false); // смотрит в другую сторону
    return ray.pos + (t - 1e-8) * ray.dir;
}

SmartPoint BeautifulPlane::get_intersection_SmartPoint(Ray ray) {
    Vec3 point = get_intersection_point(ray);
    Vec3 normal = this->normal;
    return SmartPoint(point, normal, color, material);
}
