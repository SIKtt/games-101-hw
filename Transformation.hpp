#IFNDEF TRANSFORMATION_HEADER
#DEFINE TRANSFORMATION_HEADER

#include <eigen3/Eigen/Eigen>
using namespace Eigen;

class Transformation{
    public:
    Eigen::Matrix4f get_model_matrix(float rotation_angle, int axis);



};


#ENDIF