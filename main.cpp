#include "Triangle.hpp"
#include "rasterizer.hpp"
#include "Transformation.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

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
        model_pos = Transformation::change_model_matrix(model_pos, angle, axis);
        r.set_model(model_pos);

        r.set_view(Transformation::get_view_matrix(eye_pos));
        r.set_projection(Transformation::get_projection_matrix(45, 1, 0.1, 50));

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
