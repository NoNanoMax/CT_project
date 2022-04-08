/*

    figures.cpp

Программный файл для фигур

*/

#include "scene.h"
#include "math.h"
#include <vector>

// ------------------------------- Figures -------------------------------

SmartPoint::SmartPoint(bool valid): valid(valid) { };

SmartPoint::SmartPoint(Vec3 point, Vec3 normal):
    point(point), normal(normal)
{ }

SmartPoint::SmartPoint(Vec3 point, Vec3 normal, Color color, Material material, bool valid):
    point(point), normal(normal), color(color), material(material), valid(valid)
{ }

// ------------------------------- Triangle -------------------------------

Triangle::Triangle(bool valid): valid(valid) {
    Vec3 v1(0, 0, 0), v2(0, 0, 0), v3(0, 0, 0);
    Vec3 n1(0, 0, 0), n2(0, 0, 0), n3(0, 0, 0);
    Vec3 N(0, 0, 0), U(0, 0, 0), V(0, 0, 0);
    double u0 = 0, v0 = 0;
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

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3):
    v1(v1), v2(v2), v3(v3) {
    N = vv(v2 - v1, v3 - v1).normalized();

    n1 = N, n2 = N, n3 = N;

    D = - dot(v1, N);
    Vec3 s1 = v2 - v1, s2 = v3 - v1;
    U = (s1 * dot(s2, s2) - s2 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    u0 = dot(v1, U);
    V = (s2 * dot(s1, s1) - s1 * dot(s1, s2)) / (dot(s1, s1) * dot(s2, s2) - dot(s1, s2) * dot(s1, s2));
    v0 = dot(v1, V);
}

bool Triangle::check_intersection(Ray ray) {
    if (abs(dot(N, ray.dir)) < ZERO) return false; // проверка на параллельность
    double t = - (dot(N, ray.pos) + D) / dot(N, ray.dir);
    if (t < 0) return false; // смотрит в другую сторону
    Vec3 P = ray.pos + ray.dir * t; // точка пересечения
    double u = dot(P, U) - u0; // барицентрические
    double v = dot(P, V) - v0; // координаты
    if (-ZERO <= u && u <= 1 + ZERO && -ZERO <= v && v <= 1 + ZERO && u + v <= 1 + ZERO) {
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

// ------------------------------- PolygonizedFigures -------------------------------

PolyFigure::PolyFigure() {}
PolyFigure::~PolyFigure() {
    delete body;
}

std::vector<std::string> split(std::string const &s, std::string delimeter = " ") {
    std::vector<std::string> ret;
    size_t pos = 0, old_pos = 0;
    while ((pos = s.find(delimeter, pos)) != -1) {
        ret.push_back(s.substr(old_pos, pos - old_pos));
        pos++;
        old_pos = pos;
    }
    ret.push_back(s.substr(old_pos, s.size() - pos));
    return ret;
}


#include <iostream>
Object* PolyFigure::clone(std::vector<std::string> const &arg) {
    assert(arg.size() >= 1);
    PolyFigure* rez = new PolyFigure();

    if (arg.size() >= 2) {
        rez->color = Color(arg[1]);
    }

    rez->body = new std::vector<std::vector<Triangle>>;
    rez->body->push_back(std::vector<Triangle>());
    unsigned number = 0;
    std::vector<Vec3> vertices, normals, textures;

    auto to_vec = [](std::vector<std::string> &vec) -> Vec3{
        return Vec3(atof(vec[1].c_str()), atof(vec[2].c_str()), atof(vec[3].c_str()));
    };

    auto add_tr = [&vertices, &normals, rez, number](std::vector<std::string> &vec) -> void {
        
        std::vector<std::string> f1_str = split(vec[1], "/");
        bool with_normals = (f1_str.size() == 1 ? 0 : 1);

        std::vector<Vec3> f_vertices;
        if (with_normals) {
            std::vector<Vec3> f_normals;
            // for (short i = 2; i + 1 < vec.size(); i++) {
            //     rez->body->at(number).push_back(Triangle(vertices[atoi(f_1[0].c_str()) - 1],vertices[atoi(f_2[0].c_str()) - 1], vertices[atoi(f_3[0].c_str()) - 1],
            //     normals[atoi(f_1[2].c_str()) - 1], normals[atoi(f_2[2].c_str()) - 1], normals[atoi(f_3[2].c_str()) - 1]));
            // }
        } else {
            for (short i = 1; i < vec.size(); i++) {
                f1_str = split(vec[i], "/");
                f_vertices.push_back(vertices[atoi(f1_str[0].c_str()) - 1]);
            }
            for (auto it = f_vertices.begin() + 1; it + 1 != f_vertices.end(); it++) {
                rez->body->at(number).push_back(Triangle(f_vertices[0], *it, *(it + 1)));
            }
            f_vertices.clear();
        }
    };

    std::ifstream inn(arg[0].c_str());
    if(!inn){
        perror("no input file:");
    }
    
    std::string s;
    while(getline(inn, s)) {
        std::vector<std::string> args = split(s);
        if(args.empty())
            continue;
        std::string name = args[0];
        if(name == "v") vertices.push_back(to_vec(args));
        else if(name == "vn") normals.push_back(to_vec(args));
        else if(name == "vt") textures.push_back(to_vec(args));
        else if(name == "g") ++number, rez->body->push_back(std::vector<Triangle>());
        else if(name == "f")  add_tr(args);
    }

    return rez;
}

std::pair<int, std::string> PolyFigure::name() const {
    return {FIGURE, "PolyFigure"};
}

SmartPoint PolyFigure::get_intersection_SmartPoint(Ray r) {
    double min = INFINITY; // ищем ближайшее пересечение
    Triangle tr(false);
    Vec3 point;
    Vec3 tmp;
    for(std::vector<Triangle>& bod : *body){
        for (std::vector<Triangle>::iterator it = bod.begin(); it != bod.end(); it++) {
                if (it->check_intersection(r)) {
                    tmp = it->get_intersection_point(r);
                    if ((tmp - r.pos).abs() < min) {
                        min = (tmp - r.pos).abs();
                        tr = *it;
                        point = tmp;
                    }
                }
            }
    }
    
    if (!tr.valid) return SmartPoint(false); // нет пересечения
    Vec3 normal = tr.get_normal(point);
    return SmartPoint(point, normal, color, material);
}

// ------------------------------- Sphere -------------------------------

std::pair<int,std::string> Sphere::name() const {
    return std::pair<int,std::string>(FIGURE, "Sphere");
}

SmartPoint Sphere::get_intersection_SmartPoint(Ray r) {
    if (distance(r, position) > radius) {
        return SmartPoint(false);
    }
    double min = INFINITY; // ищем ближайшее пересечение
    Triangle tr(false);
    Vec3 point;
    Vec3 tmp;
    for (std::vector<Triangle>::iterator it = body.begin(); it != body.end(); it++) {
        if (it->check_intersection(r)) {
            tmp = it->get_intersection_point(r);
            if ((tmp - r.pos).abs() < min) {
                min = (tmp - r.pos).abs();
                tr = *it;
                point = tmp;
            }
        }
    }
    if (!tr.valid) return SmartPoint(false); // нет пересечения
    Vec3 normal = tr.get_normal(point);
    return SmartPoint(point, normal, color, material);
}

Object* Sphere::clone(std::vector<std::string> const & arg) {
    assert(arg.size() >= 4);
    Sphere* rez = new Sphere();
    Vec3 center =  Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    double radius = atof(arg[3].c_str());
    rez->radius = radius;
    rez->position = center;
    if (arg.size() >= 5) rez->color = Color(arg[4]);
    if (arg.size() >= 6) rez->material = Material(arg[5]);
    unsigned iterations = 2;
    if (arg.size() >= 7) iterations = atoi(arg[6].c_str());

    rez->body.push_back(Triangle(center + Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)));
    rez->body.push_back(Triangle(center + Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, -1)));
    rez->body.push_back(Triangle(center + Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, 1)));
    rez->body.push_back(Triangle(center + Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1)));
    rez->body.push_back(Triangle(center - Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)));
    rez->body.push_back(Triangle(center - Vec3(radius, 0, 0), center + Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, -1)));
    rez->body.push_back(Triangle(center - Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center + Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, 1)));
    rez->body.push_back(Triangle(center - Vec3(radius, 0, 0), center - Vec3(0, radius, 0), center - Vec3(0, 0, radius), Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 0, -1)));

    std::vector<Triangle> next;
    for (unsigned j = 0; j < iterations; j++) {
        unsigned amount = rez->body.size();
        for (unsigned i = 0; i < amount; i++) {
            Vec3 v1 = ((rez->body[i].v1 + rez->body[i].v2) / 2 - center).normalized() * radius + center;
            Vec3 v2 = ((rez->body[i].v2 + rez->body[i].v3) / 2 - center).normalized() * radius + center;
            Vec3 v3 = ((rez->body[i].v1 + rez->body[i].v3) / 2 - center).normalized() * radius + center;
            next.push_back(Triangle(v1, v2, v3, (v1 - center).normalized(), (v2 - center).normalized(), (v3 - center).normalized()));
            next.push_back(Triangle(rez->body[i].v1, v1, v3, (rez->body[i].v1 - center).normalized(), (v1 - center).normalized(), (v3 - center).normalized()));
            next.push_back(Triangle(rez->body[i].v2, v1, v2, (rez->body[i].v2 - center).normalized(), (v1 - center).normalized(), (v2 - center).normalized()));
            next.push_back(Triangle(rez->body[i].v3, v2, v3, (rez->body[i].v3 - center).normalized(), (v2 - center).normalized(), (v3 - center).normalized()));
        }
        rez->body = next;
        next.clear();
    }

    return rez;
}

// ------------------------------- Tetraedr -------------------------------

std::pair<int,std::string> Tetraedr::name() const{
    return std::pair<int,std::string>(FIGURE, "Tetraedr");
}

SmartPoint Tetraedr::get_intersection_SmartPoint(Ray r) {
    double min = INFINITY; // ищем ближайшее пересечение
    Triangle tr(false);
    Vec3 point;
    Vec3 tmp;
    for (std::vector<Triangle>::iterator it = body.begin(); it != body.end(); it++) {
        if (it->check_intersection(r)) {
            tmp = it->get_intersection_point(r);
            if ((tmp - r.pos).abs() < min) {
                min = (point - r.pos).abs();
                tr = *it;
                point = tmp;
            }
        }
    }
    if (!tr.valid) return SmartPoint(false); // нет пересечения
    Vec3 normal = tr.get_normal(point);
    return SmartPoint(point, normal, color, material);
}

// v1.x v1.y v1.z [ v2.x v2.y v2.z v3.x v3.y v3.z v4.x v4.y v4.z ]
Object* Tetraedr::clone(std::vector<std::string> const & arg) {
    assert(arg.size() >= 3);
    Tetraedr* rez = new Tetraedr();
    Vec3 pos =  Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    Vec3 a(-2, 1, -2);
    Vec3 b(0, -2, -2);
    Vec3 c(2, 1, -2);
    if (arg.size() == 4 || arg.size() == 5) {
        rez->color = Color(arg[3]);
    }
    if (arg.size() == 5) {
        rez->material = Material(arg[4]);
    }
    if (arg.size() >= 12) {
        a =  Vec3(atof(arg[3].c_str()),atof(arg[4].c_str()),atof(arg[5].c_str()));
        b =  Vec3(atof(arg[6].c_str()),atof(arg[7].c_str()),atof(arg[8].c_str()));
        c =  Vec3(atof(arg[9].c_str()),atof(arg[10].c_str()),atof(arg[11].c_str()));
    }
    if (arg.size() >= 13) rez->color = Color(arg[12]);
    if (arg.size() >= 14) rez->material = Material(arg[13]);
    
    Vec3 n1 = (dot(vv(a, b), c) < 0 ? vv(a, b).normalized() : vv(b, a).normalized());
    rez->body.push_back(Triangle(pos, pos + a, pos + b, n1, n1, n1));

    Vec3 n2 = (dot(vv(b, c), a) < 0 ? vv(b, c).normalized() : vv(c, b).normalized());
    rez->body.push_back(Triangle(pos, pos + b, pos + c, n2, n2, n2));

    Vec3 n3 = (dot(vv(a, c), b) < 0 ? vv(a, c).normalized() : vv(c, a).normalized());
    rez->body.push_back(Triangle(pos, pos + a, pos + c, n3, n3, n3));

    Vec3 d = a - b;
    Vec3 e = a - c;
    Vec3 n4 = (dot(vv(d, e), Vec3(0, 0, 0) - a) < 0 ? vv(d, e).normalized() : vv(e, d).normalized());
    rez->body.push_back(Triangle(pos + a, pos + b, pos + c, n4, n4, n4));

    return rez;
}

// ------------------------------- BeautifulSphere -------------------------------

Object* BeautifulSphere::clone(std::vector<std::string> const &arg) {
    assert(arg.size() >= 4);
    BeautifulSphere* rez = new BeautifulSphere();
    rez->position =  Vec3(atof(arg[0].c_str()),atof(arg[1].c_str()),atof(arg[2].c_str()));
    rez->radius = atof(arg[3].c_str());
    if (arg.size() >= 5) rez->color = Color(arg[4]);
    if (arg.size() >= 6) rez->material = Material(arg[5]);
    return rez;
}

std::pair<int,std::string> BeautifulSphere::name() const{
    return std::pair<int,std::string>(FIGURE, "BeautifulSphere");
}

SmartPoint BeautifulSphere::get_intersection_SmartPoint(Ray ray) {
    SmartPoint ret = SmartPoint(false);
    double b = 2 * dot(ray.dir, ray.pos - position);
    double c = dot(ray.pos - position, ray.pos - position) - radius * radius;
    double delta = b * b - 4 * c; // D
    if (delta > 0) {
        double t1 = (- b + sqrt(delta)) / 2;
        double t2 = (- b - sqrt(delta)) / 2;
        if (t1 > 0 || t2 > 0) {
            if (t1 > 0 and t2 > 0 and t1 > t2) t1 = t2;
            if (t1 < 0) t1 = t2;
            ret.point = ray.pos + t1 * ray.dir;
            ret.valid = true;
        }
    }
    if (!ret.valid) {
        return ret;
    }
    ret.normal = (ret.point - this->position).normalized();
    ret.color = color;
    ret.material = material;
    return ret;
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
    if (arg.size() >= 6) rez->material = Material(arg[5]);
    return rez;
}

SmartPoint BeautifulPlane::get_intersection_SmartPoint(Ray ray) {
    SmartPoint ret = SmartPoint(false);
    double k = dot(normal, ray.dir);
    if (k == 0) return ret; // параллельность

    double t = (this->d - dot(ray.pos, normal)) / k;
    if (t < 0) return ret; // смотрит в другую сторону

    ret.valid = true;
    ret.point = ray.pos + (t - 1e-8) * ray.dir;
    ret.normal = this->normal;
    ret.color = color;
    ret.material = material;
    return ret;
}
