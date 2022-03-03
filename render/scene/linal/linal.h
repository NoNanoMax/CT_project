/*

    linal.h

Низкоуровневое описание математический объектов

*/

class Vec3 {
public:
    double x, y, z;
    Vec3();
    Vec3(double x, double y, double z);
    double abs();
    void normalize();
    Vec3 normalized();
    Vec3 operator+(Vec3 other);
    Vec3 operator-(Vec3 other);
    Vec3 operator*(double other);
    Vec3 operator/(double other);
};

double dot(Vec3 a, Vec3 b);
Vec3 vec(Vec3 a, Vec3 b);

class Ray {
public:
    Vec3 from, to;
    Ray(Vec3 from, Vec3 to);
};

class Matr3 {
public:
    double body[3][3];
    Matr3();
    Matr3(Vec3 a, Vec3 b, Vec3 c);
    Matr3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33);
    void trans();
    Matr3 operator+(Matr3 other);
    Matr3 operator-(Vec3 other);
    Matr3 operator*(Matr3 other);
    Vec3 operator*(Vec3 other);
};

Vec3 dot(Matr3 m, Vec3 v);

