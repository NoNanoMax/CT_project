/*

    material.h

Программный файл для material.h

*/

#include <string>
#include "material.h"

Material::Material(std::string name, double n): n(n) {
    if (name == "dielectric"){
        t_reflection = 0;
        t_refraction = 0;
        t_diffusion = 1;
    }
    if (name == "metall") {
        t_reflection = 0.3;
        t_refraction = 0;
        t_diffusion = 0.7;
    }
    if (name == "glass") {
        t_reflection = 0.1;
        t_refraction = 0.9;
        t_diffusion = 0;
    }
    if (name == "mirror") {
        t_reflection = 1;
        t_refraction = 0;
        t_diffusion = 0;
    }
}

Material::Material(double t_reflection, double t_refraction, double t_diffusion, double n):
    n(n), t_reflection(t_reflection), t_refraction(t_reflection), t_diffusion(t_reflection) 
{ }

void Color::colorize(std::string name) {
    if (name == "white") r = 255, g = 255, b = 255;
    if (name == "black") r = 0, g = 0, b = 0;
    if (name == "red") r = 255, g = 0, b = 0;
    if (name == "green") r = 0, g = 255, b = 0;
    if (name == "blue") r = 0, g = 0, b = 255;
    if (name == "yellow") r = 255, g = 255, b = 0;
    if (name == "grey") r = 96, g = 96, b = 96;
    if (name == "deep_purple") r = 102, g = 0, b = 102;
    if (name == "orange") r = 255, g = 128, b = 0;
}

// ненормированная интенсивность для каждого цвета (float (не от 0 до 255))
Color::Color() {
    r = 0, g = 0, b = 0;
}

Color::Color(std::string name) {
    colorize(name);
}

Color::Color(double r, double g, double b):
    r(r), g(g), b(b) 
{ }



