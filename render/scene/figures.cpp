/*

    figures.cpp

Программный файл для figures.h

*/

#include "scene.h"

// bool Figure::check_intersection(Ray r) {
//     return false;
// }

Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3): v1(v1), v2(v2), v3(v3) { }

bool Triangle::check_intersection(Ray r) {
    if (2 == 2) {
        return true;
    }
    return false;
}

