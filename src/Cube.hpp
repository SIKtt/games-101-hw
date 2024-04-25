#ifndef RASTERIZER_CUBE_H
#define RASTERIZER_CUBE_H

#include <eigen3/Eigen/Eigen>

using namespace Eigen;

class Cube
{
    public:
        Eigen::Matrix<float, 3, 1> v[8]; //Cube all 8 vertex (3 al least if direction fixed)
        /*Per vertex values*/
        Eigen::Matrix<float, 3, 1> color[8];      // color at each vertex;
        //Vector3f tex_coords[3]; // texture u,v
        //Eigen::Matrix<float, 8, 1> normal[8];     // normal vector for each vertex

        // Texture *tex; //waiting
        Cube();

        // Eigen::Vector3f a() const { return v[0]; }
        // Eigen::Vector3f b() const { return v[1]; }
        // Eigen::Vector3f c() const { return v[2]; }
        Eigen::Vector3f bbl() const { return v[0]; } //bottom surf - bottom line - left point
        Eigen::Vector3f bbr() const { return v[1]; }
        Eigen::Vector3f btr() const { return v[2]; }
        Eigen::Vector3f btl() const { return v[3]; }
        Eigen::Vector3f tbl() const { return v[4]; } //bottom surf - bottom line - left point
        Eigen::Vector3f tbr() const { return v[5]; }
        Eigen::Vector3f ttr() const { return v[6]; }
        Eigen::Vector3f ttl() const { return v[7]; }

        void setVertex(int ind, Vector3f ver); /*set i-th vertex coordinates */
        // void setNormal(int ind, VectorXf n);   /*set i-th vertex normal vector*/
        void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/
        // void setTexCoord(int ind, float s,float t); /*set i-th vertex texture coordinate*/
        // std::array<Vector4f, 3> toVector4() const;
};

#endif