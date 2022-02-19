/*

    linal.cpp

Програмный файл для linal.h

*/

#include "linal.h"
#include "math.h"

Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) {}

double Vec3::abs() {
    return sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
    //this->x = x / abs;
}


