/*

    linal.cpp

Програмный файл для linal.h

*/

#include "linal.h"
#include "math.h"

Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) { };

double Vec3::abs() {
    return sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
    double a = abs();
    x = x / a;
    y = y / a;
    z = z / a;
}

Vec3 Vec3::operator+(Vec3 other) {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(Vec3 other) {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

double dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec(Vec3 a, Vec3 b) {
    double new_x = a.y * b.z - a.z * b.y;
    double new_y = a.z * b.x - a.x * b.z;
    double new_z = a.x * b.y - a.y * b.x;
    return Vec3(new_x, new_y, new_z);
}

Ray::Ray(Vec3 from, Vec3 to): from(from), to(to) { };




