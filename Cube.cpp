
#include "Cube.hpp"
#include <eigen3/Eigen/Eigen>

using namespace Eigen;

Cube::Cube{
    v[0] << 0, 0, 0;
    v[1] << 0, 0, 0;
    v[2] << 0, 0, 0;
    v[3] << 0, 0, 0;
    v[4] << 0, 0, 0;
    v[5] << 0, 0, 0;
    v[6] << 0, 0, 0;
    v[7] << 0, 0, 0;
}

void Cube::setVertex(int ind, Eigen::Vector3f ver) { v[ind] = ver; }

//void Cube::setNormal(int ind, Vector3f n) { normal[ind] = n; }

void Cube::setColor(int ind, float r, float g, float b)
{
    if ((r < 0.0) || (r > 255.) || (g < 0.0) || (g > 255.) || (b < 0.0) ||
        (b > 255.)) { // waiting for better experisson
        throw std::runtime_error("Invalid color values");
    }

    color[ind] = Vector3f((float)r / 255., (float)g / 255., (float)b / 255.);
    return;
}