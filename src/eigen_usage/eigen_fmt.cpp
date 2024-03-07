#include <Eigen/Dense>

#define FMT_DEPRECATED_OSTREAM
#include <spdlog/spdlog.h>

#include <fmt/std.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/ranges.h>

#include <concepts>
#include <filesystem>

#define ICINFO(expr) SPDLOG_INFO("{}: {}", #expr, expr)

namespace fs = std::filesystem;

FMT_BEGIN_NAMESPACE
template <typename T, typename Char>
    requires std::is_base_of_v<Eigen::DenseBase<T>, T> || std::is_same_v<fs::path, T>
struct is_range<T, Char>
{
    static constexpr const bool value = false;
};

template <typename T, typename Char> struct is_range<Eigen::MatrixBase<T>, Char>
{
    static constexpr const bool value = false;
};
FMT_END_NAMESPACE

using MatT = Eigen::MatrixBase<Eigen::Block<const Eigen::Matrix<double, 7, 1, 2, 7, 1>, -1, -1, false>>;
int main()
{
    fmt::is_range<Eigen::MatrixXd, char>::value;
    fmt::is_range<fs::path, char>::value;
    fmt::is_range<MatT, char>::value;

    fs::path p1 = fs::current_path();
    ICINFO(p1);

    Eigen::VectorXd vec;
    vec.setRandom(3);
    SPDLOG_INFO("vec: {}", vec);

    Eigen::Vector3d vec2;
    vec2.setRandom();
    SPDLOG_INFO("vec2: {}", vec2);

    Eigen::MatrixXd m1;
    m1.setRandom(3, 3);
    SPDLOG_INFO("m1: {}", m1);
    auto m2 = m1.topLeftCorner<2, 2>();
    std::is_base_of<Eigen::MatrixBase<decltype(m2)>, decltype(m2)>::value;
    std::is_base_of<Eigen::MatrixBase<MatT>, MatT>::value;
    std::is_base_of<Eigen::MatrixBase<Eigen::MatrixXd>, Eigen::MatrixXd>::value;
    std::is_base_of<Eigen::DenseBase<MatT>, MatT>::value;
    SPDLOG_INFO("m1: {}", m2);

    Eigen::VectorXd v1(3);
    v1 << 1.2, 2.1, 2.2;
    ICINFO(v1.dot(Eigen::VectorXd::Ones(3)));
    return 0;
}