#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>

struct Material
{
    std::string name;
    double effectiveMass;
    double bandGap;
    double dielectricConstant;
};

#endif