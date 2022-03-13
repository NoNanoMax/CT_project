#include "linal.cpp"

//returns -1 or t: r0 + a*t lying in triangle
double intersection(Vec3 pos, Vec3 dir, Vec3 v0, Vec3 v1, Vec3 v2) {
    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    // Вычисление вектора нормали к плоскости
    Vec3 pvec = vec(dir, e2);
    float det = dot(e1, pvec);

    // Луч параллелен плоскости
    if (det < 1e-8 && det > -1e-8) {
        return 0;
    }

    float inv_det = 1 / det;
    Vec3 tvec = pos - v0;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
        return -1;
    }

    Vec3 qvec = vec(tvec, e1);
    float v = dot(dir, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
        return -1;
    }
    return dot(e2, qvec) * inv_det;
 
}