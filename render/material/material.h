/*

    material.h

Здесь описываются материалы и цвета

*/

#include <string>

// типы материалов "dielectric", "metall", "glASS"
class Material {
public:
    double t_reflection, t_refraction, t_diffusion;
    double n; // показатель преломления
    Material(std::string name, double n = 1.33);
    Material(double t_reflection, double t_refraction, double t_diffusion, double n = 1.33);
};

class Color {
public:
    double r, g, b;
    Color();
    Color(std::string name);
    Color(double r, double g, double b);
    void colorize(std::string name);
};






