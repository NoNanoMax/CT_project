/*

    linal.h

Низкоуровневое описание математический объектов

*/

class Vec3 {
public:
    double x;
    double y;
    double z;
    Vec3(double x, double y, double z);
    double abs();
    void normalize();
};


