/*

    material.h

Программный файл для material.h

*/

#include <string>
#include "material.h"

Material::Material(std::string name, double n): n(n) {
    if (name == "dielectric"){
        t_reflection = 0.05;
        t_refraction = 0;
        t_diffusion = 0.95;
    }
    if (name == "metall") {
        t_reflection = 0.2;
        t_refraction = 0;
        t_diffusion = 0.8;
    }
    if (name == "glass") {
        t_reflection = 0.1;
        t_refraction = 0.9;
        t_diffusion = 0;
    }
}

Material::Material(double t_reflection, double t_refraction, double t_diffusion, double n):
    n(n), t_reflection(t_reflection), t_refraction(t_reflection), t_diffusion(t_reflection) 
{ }

void Color::make(std::string name) {
    if (name == "white") r = 255, g = 255, b = 255;
    if (name == "black") r = 0, g = 0, b = 0;
    if (name == "red") r = 255, g = 0, b = 0;
    if (name == "green") r = 0, g = 255, b = 0;
    if (name == "blue") r = 0, g = 0, b = 255;
}

Color::Color() {
    //make("black");
    r = 0, g = 0, b = 0;
}

Color::Color(std::string name) {
    make(name);
}

Color::Color(short r, short g, short b):
    r(r), g(g), b(b) 
{ }



