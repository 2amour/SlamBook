#include<iostream>
#include <cmath>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "sophus/so3.h"
#include "sophus/se3.h"


/**
 * 本程序演示了sophus中SO(3)与SE(3)的使用
 * 注意,从GitHub上下载完Sophus之后要修改 sophus/so2.cpp 文件,将其中的
 * unit_complex_.real() = 1.;
 * unit_complex_.imag() = 0.;
 * 修改为
 * unit_complex_ = Complexd(1.,0.);
 * 否则在Mac上无法编译
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {

    //声明一个沿Z轴旋转90度的旋转矩阵
    Eigen::Matrix3d R = Eigen::AngleAxisd(M_PI / 2, Eigen::Vector3d(0, 0, 1)).toRotationMatrix();
//    从旋转矩阵直接构造SO(3)
    Sophus::SO3 SO3_R(R);
//    从旋转向量构造
    Sophus::SO3 SO3_v(0, 0, M_PI / 2);
//    从四元数构造
    Eigen::Quaterniond q(R);
    Sophus::SO3 SO3_q(q);

    cout << "SO(3) from matrix: " << SO3_R << endl;
    cout << "SO(3) from vector: " << SO3_v << endl;
    cout << "SO(3) from quaternion: " << SO3_q << endl;

//    使用对数映射得到李代数
    Eigen::Vector3d so3 = SO3_R.log();
    cout << "so(3) = " << so3.transpose() << endl;
//    将so(3)以反对称矩阵的形式输出
    cout << "so(3) hat = \n" << Sophus::SO3::hat(so3) << endl;
//    将反对称矩阵转回so(3)
    cout << "so(3) hat vee = " << Sophus::SO3::vee(Sophus::SO3::hat(so3)).transpose() << endl;

//    利用so(3)扰动模型来更新位姿
    Eigen::Vector3d update_so3(1e-4, 0, 0);
//    左乘更新
    Sophus::SO3 SO3_updated = Sophus::SO3::exp(update_so3) * SO3_R;
    cout << "SO(3) updated = " << SO3_updated << endl;


//    SE(3)
    // 平移向量
    Eigen::Vector3d t(1, 0, 0);
//    根据R,t构造SE(3)
    Sophus::SE3 SE3_Rt(R, t);
    cout << "SE(3) from R,t =\n " << SE3_Rt << endl;
//    根据q,t构造SE(3)
    Sophus::SE3 SE3_qt(q, t);
    cout << "SE(3) from q,t =\n " << SE3_qt << endl;

//    se(3)是一个六维向量，方便起见，使用typedef
    typedef Eigen::Matrix<double, 6, 1> Vector6d;
    Vector6d se3 = SE3_Rt.log();
    cout << "se(3) = " << se3.transpose() << endl;
    cout << "se(3) hat = \n" << Sophus::SE3::hat(se3) << endl;
    cout << "se(3) hat vee = " << Sophus::SE3::vee(Sophus::SE3::hat(se3)).transpose() << endl;

//    利用se(3)扰动模型来更新位姿
    Vector6d update_se3;
    update_se3.setZero();
    update_se3(0, 0) = 1e-4;
//    cout<<"update_se3 = "<<update_se3.transpose()<<endl;
    Sophus::SE3 SE3_updated = Sophus::SE3::exp(update_se3) * SE3_Rt;
    cout << "SE(3) updated = \n" << SE3_updated.matrix() << endl;
    return 0;
}
