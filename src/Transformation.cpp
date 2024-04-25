#include "Transformation.hpp"

#define MY_PI 3.1415926

Eigen::Matrix4f Transformation::get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;

    translate << 
        1, 0, 0, -eye_pos[0], 
        0, 1, 0, -eye_pos[1], 
        0, 0, 1, -eye_pos[2],
        0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f Transformation::get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    float angle = eye_fov * MY_PI / 180.0; // half angle
	float height = zNear * tan(angle) * 2;
	float width = height * aspect_ratio;

    Eigen::Matrix4f perspective_proj;

    auto t = -zNear * std::tan(angle / 2);
	auto r = t * aspect_ratio;
	auto l = -r;
	auto b = -t;
    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    perspective_proj <<
        2*zNear/(r-l), 0, (l+r)/(l-r), 0,
        0, 2*zNear/(t-b), (b+t)/(b-t), 0,
        0, 0, (zFar+zNear)/(zNear-zFar), 2*zFar*zNear/(zFar-zNear),
        0, 0, 1, 0;

    projection = perspective_proj * projection;

    return projection;
}

Eigen::Matrix4f Transformation::change_model_matrix(Eigen::Matrix4f model_matrix, float rotation_angle, int axis)
{

    float cos_phi = std::cos(rotation_angle);
    float sin_phi = std::sin(rotation_angle);

    Eigen::Matrix4f rotation;
    if (axis == -1)
    {
        return model_matrix;
    }

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


    model_matrix = rotation * model_matrix;

    return model_matrix;
}

Eigen::Matrix4f Transformation::get_model_matrix(float rotation_angle, int axis)
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
