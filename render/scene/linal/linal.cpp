/*

    linal.cpp

Програмный файл для linal.h

*/

#include "linal.h"
#include "math.h"

// --------------------------------------------- Vec3 ---------------------------------------------

Vec3::Vec3() {
    x = 0, y = 0, z = 0;
}

Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) { };

double Vec3::abs() const {
    return sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
    double a = abs();
    x = x / a, y = y / a, z = z / a;
}

Vec3 Vec3::normalized() {
    return Vec3 (x / abs(), y / abs(), z / abs());
}

Vec3 Vec3::operator+(Vec3 other) {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(Vec3 other) {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

double Vec3::operator[](int const& index) const {
    if(index == 0) return x;
    else if(index == 1) return y;
    else return z;
}

Vec3 operator*(const Vec3& a,const double b) {
    return Vec3(a.x * b, a.y * b, a.z * b);
}

Vec3 operator*(const double b,const Vec3& a) {
    return Vec3(a.x * b, a.y * b, a.z * b);
}

Vec3 Vec3::operator/(double other) {
    return Vec3(x / other, y / other, z / other);
}

double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vv(const Vec3& a, const Vec3& b) {
    double new_x = a.y * b.z - a.z * b.y;
    double new_y = a.z * b.x - a.x * b.z;
    double new_z = a.x * b.y - a.y * b.x;
    return Vec3(new_x, new_y, new_z);
}
double cos(const Vec3& a, const Vec3& b) {
    return dot(a, b) / a.abs() / b.abs();
}

// --------------------------------------------- Ray ---------------------------------------------

Ray::Ray(Vec3 from, Vec3 dir, unsigned x, unsigned y, double intensity):
     from(from), dir(dir), x(x), y(y), intensity(intensity) { }

// --------------------------------------------- Matr3 ---------------------------------------------

Matr3::Matr3() {
    for(short i = 0; i < 3; i++) {
        for(short j = 0; j < 3; j++) {
            body[i][j] = 0;
        }
    }
}

Matr3::Matr3(Vec3 v1, Vec3 v2, Vec3 v3) {
    body[0][0] = v1.x, body[0][1] = v2.x, body[0][2] = v3.x;
    body[1][0] = v1.y, body[1][1] = v2.y, body[1][2] = v3.y;
    body[2][0] = v1.z, body[2][1] = v2.z, body[2][2] = v3.z;
}

Matr3::Matr3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33) {
    body[0][0] = a11, body[0][1] = a12, body[0][2] = a13;
    body[1][0] = a21, body[1][1] = a22, body[1][2] = a23;
    body[2][0] = a31, body[2][1] = a32, body[2][2] = a33;
}

void Matr3::trans() {
    for(short i = 0; i < 3; i++) {
        for(short j = i + 1; j < 3; j++) {
            double t = body[i][j];
            body[i][j] = body[j][i];
            body[j][i] = t;
        }
    }
}

Matr3 Matr3::operator+(Matr3 other) {
    Matr3 ret;
    for(short i = 0; i < 3; i++) {
        for(short j = 0; j < 3; j++) {
            ret.body[i][j] = body[i][j] + other.body[i][j];
        }
    }
    return ret;
}

Matr3 Matr3::operator*(Matr3 other) {
   Matr3 ret;
    for(short i = 0; i < 3; i++) {
        for(short j = 0; j < 3; j++) {
            double t = 0;
            for(short k = 0; k < 3; k++) {
                t += body[i][k] * other.body[k][j]; 
            }
            ret.body[i][j] = t;
        }
    }
    return ret;
}

Vec3 Matr3::operator*(Vec3 other) {
    Vec3 ret;
    ret.x = body[0][0] * other.x + body[0][1] * other.y + body[0][2] * other.z;
    ret.y = body[1][0] * other.x + body[1][1] * other.y + body[1][2] * other.z;
    ret.z = body[2][0] * other.x + body[2][1] * other.y + body[2][2] * other.z;
    return ret;
}

