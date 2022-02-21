/*

    linal.h

Низкоуровневое описание математический объектов

*/

class Vec3 {
public:
    double x, y, z;
    Vec3(double x, double y, double z);
    double abs();
    void normalize();
};

class Ray {
public:
    Vec3 from, to;
    Ray(Vec3 from, Vec3 to);
};

