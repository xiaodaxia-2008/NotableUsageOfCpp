#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <numeric>
#include <string_view>

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

int main()
{
    Eigen::Isometry3d m = Eigen::Isometry3d::Identity();
    m.translate(Eigen::Vector3d(0.5, 1.5, 2.5));
    m.rotate(Eigen::AngleAxisd(EIGEN_PI / 2, Eigen::Vector3d::UnitZ()));

    std::cout << m.matrix() << std::endl;

    Eigen::Vector3d t(1, 0.5, 1.5);
    std::cout << m * t << std::endl;

    auto m2 = m * Eigen::Translation3d(t);
    std::cout << m2.matrix() << std::endl;

    auto m3 = Eigen::Translation3d(t) * m;
    std::cout << m3.matrix() << std::endl;
    return 0;
}