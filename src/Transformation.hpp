#ifndef TRANSFORMATION_HEADER
#define TRANSFORMATION_HEADER

#include <eigen3/Eigen/Eigen>
using namespace Eigen;

class Transformation
{
    public:
        static Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
        static Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,float zNear, float zFar);
        static Eigen::Matrix4f change_model_matrix(Eigen::Matrix4f model_matrix, float rotation_angle, int axis);
        static Eigen::Matrix4f get_model_matrix(float rotation_angle, int axis);

};


#endif