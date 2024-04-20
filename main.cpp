#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
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

Eigen::Matrix4f change_model_matrix(Eigen::Matrix4f model_matrix, float rotation_angle, int axis)
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

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
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

int main(int argc, const char** argv)
{
    float angle = 0;
    int axis = -1;
    bool command_line = false;
    std::string filename = "output.png";

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 10};

    Eigen::Matrix4f model_pos = Eigen::Matrix4f::Identity();

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        //r.set_model(get_model_matrix(angle, axis));
        model_pos = change_model_matrix(model_pos, angle, axis);
        r.set_model(model_pos);

        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);


        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        axis = -1; // Nothin change

        if (key == 'a') {
            axis = 2;
            angle = +MY_PI/90.0;
        }
        else if (key == 'd') {
            axis = 2;
            angle = -MY_PI/90.0;
        }
        else if (key == 'w') {
            axis = 0;
            angle = +MY_PI/90.0;
        }
        else if (key == 's') {
            axis = 0;
            angle = -MY_PI/90.0;
        }
    }

    return 0;
}
