
#include <Transformation.hpp>


Eigen::Matrix4f get_model_matrix(float rotation_angle, int axis)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float cos_phi = std::cos(rotation_angle);
    float sin_phi = std::sin(rotation_angle);

    Eigen::Matrix4f rotation;
    if (axis==0) // x-axis
    {
    rotation << 
        1, 0, 0, 0,
        0, cos_phi, -sin_phi, 0,
        0, sin_phi, cos_phi, 0,
        0, 0, 0, 1;
    }
    if (axis==1)
    {
    rotation << 
        cos_phi, 0, sin_phi, 0,
        0, 1, 0, 0,
        -sin_phi, 0, cos_phi, 0,
        0, 0, 0, 1;
    }
    if (axis==2) // z-axis
    {
    rotation << 
        cos_phi, -sin_phi, 0, 0, 
        sin_phi, cos_phi, 0, 0, 
        0, 0, 1, 0,
        0, 0, 0, 1;
    }

    model = rotation * model;

    return model;
}
