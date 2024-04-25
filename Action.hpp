#ifndef ACTION_COMMON_H
#define ACTION_COMMON_H

#include <eigen3/Eigen/Eigen>
using namespace Eigen;

class action
{
    public:
        Eigen::Matrix<float, 4, 4> drop(Eigen::Matrix<float, 4, 4>);
        Eigen::Matrix<float, 4, 4> clean(Eigen::Matrix<float, 4, 4>);

}

#endif