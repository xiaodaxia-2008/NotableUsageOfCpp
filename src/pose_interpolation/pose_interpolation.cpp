#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#define IC(...) SPDLOG_DEBUG(#__VA_ARGS__ ": {}", __VA_ARGS__)
#define FMT_DEPRECATED_OSTREAM
#include <spdlog/spdlog.h>

#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <concepts>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

FMT_BEGIN_NAMESPACE
template <typename T, typename Char>
    requires std::is_base_of_v<Eigen::DenseBase<T>, T>
             || std::is_base_of_v<Eigen::MatrixBase<T>, T>
             || std::is_same_v<fs::path, T>
struct is_range<T, Char>
{
    static constexpr const bool value = false;
};
template <typename T, typename Char>
struct is_range<Eigen::MatrixBase<T>, Char>
{
    static constexpr const bool value = false;
};

template <typename _Scalar, int _Dim, int _Mode, int _Options>
struct formatter<Eigen::Transform<_Scalar, _Dim, _Mode, _Options>>
    : formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const Eigen::Transform<_Scalar, _Dim, _Mode, _Options> &t,
                FormatContext &ctx) const
    {
        return fmt::format_to(ctx.out(), "Transform:\n{}", t.matrix());
    }
};
FMT_END_NAMESPACE

template <std::floating_point Scalar>
Eigen::Vector3<Scalar> operator-(const Eigen::Quaternion<Scalar> &lhs,
                                 const Eigen::Quaternion<Scalar> &rhs)
{
    Eigen::AngleAxis<Scalar> aa(rhs.conjugate() * lhs);
    return aa.angle() * aa.axis();
}

template <std::floating_point Scalar>
Eigen::Quaternion<Scalar> operator+(const Eigen::Quaternion<Scalar> &lhs,
                                    const Eigen::Quaternion<Scalar> &rhs)
{
    return lhs * rhs;
}

template <std::floating_point Scalar,
          std::convertible_to<Eigen::Vector3<Scalar>> T>
Eigen::Quaternion<Scalar> operator+(const Eigen::Quaternion<Scalar> &lhs,
                                    const T &so3)
{
    Scalar angle = so3.norm();
    auto axis = so3.normalized();
    return lhs * Eigen::AngleAxis<Scalar>(angle, axis);
}

template <std::floating_point Scalar>
Eigen::Vector<Scalar, 6>
operator-(const Eigen::Transform<Scalar, 3, Eigen::Isometry> &lhs,
          const Eigen::Transform<Scalar, 3, Eigen::Isometry> &rhs)
{
    using Quat = Eigen::Quaternion<Scalar>;
    Eigen::Vector<double, 6> se3;
    // convert to rhs's local frame
    se3.segment<3>(0) =
        rhs.rotation().transpose() * (lhs.translation() - rhs.translation());
    se3.segment<3>(3) = Quat(lhs.rotation()) - Quat(rhs.rotation());
    return se3;
}

template <std::floating_point Scalar,
          std::convertible_to<Eigen::Vector<Scalar, 6>> T>
Eigen::Transform<Scalar, 3, Eigen::Isometry>
operator+(const Eigen::Transform<Scalar, 3, Eigen::Isometry> &lhs, const T &se3)
{
    Eigen::Transform<Scalar, 3, Eigen::Isometry> p(lhs);
    p.translate(se3.segment<3>(0));
    Scalar angle = se3.segment<3>(3).norm();
    auto axis = se3.segment<3>(3).normalized();
    p.rotate(Eigen::AngleAxis<Scalar>(angle, axis));
    return p;
}

template <std::floating_point Scalar,
          std::convertible_to<Eigen::Vector<Scalar, 6>> T>
Eigen::Transform<Scalar, 3, Eigen::Isometry>
operator+(const T &se3, const Eigen::Transform<Scalar, 3, Eigen::Isometry> &rhs)
{
    Eigen::Transform<Scalar, 3, Eigen::Isometry> p(rhs);
    p.pretranslate(se3.segment<3>(0));
    Scalar angle = se3.segment<3>(3).norm();
    auto axis = se3.segment<3>(3).normalized();
    p.prerotate(Eigen::AngleAxis<Scalar>(angle, axis));
    return p;
}

template <std::floating_point Scalar>
Eigen::Transform<Scalar, 3, Eigen::Isometry>
operator+(const Eigen::Transform<Scalar, 3, Eigen::Isometry> &lhs,
          const Eigen::Transform<Scalar, 3, Eigen::Isometry> &rhs)
{
    return lhs * rhs;
}

int main()
{
    spdlog::set_level(spdlog::level::debug);
    constexpr double eps = 1e-10;
    using Isometry3d = Eigen::Isometry3d;
    const Eigen::Quaterniond q1 = Eigen::Quaterniond::UnitRandom();
    const Eigen::Quaterniond q2 = Eigen::Quaterniond::UnitRandom();
    Isometry3d p1 = Isometry3d::Identity();
    Isometry3d p2 = Isometry3d::Identity();
    p1.translation().setRandom();
    p2.translation().setRandom();
    p1.linear() = q1.toRotationMatrix();
    p2.linear() = q2.toRotationMatrix();
    IC(p1);
    IC(p2);
    IC(p2 - p1);
    IC(p1 + (p2 - p1));


    // for (int i = 0; i < 100; ++i) {
    //     const Eigen::Quaterniond q1 = Eigen::Quaterniond::UnitRandom();
    //     const Eigen::Quaterniond q2 = Eigen::Quaterniond::UnitRandom();
    //     Eigen::Quaterniond q11(-q1.coeffs());
    //     IC(q1.coeffs().transpose());
    //     IC(q11.coeffs().transpose());

    //     const auto R1 = q1.toRotationMatrix();
    //     const auto R11 = q11.toRotationMatrix();
    //     const auto R2 = q2.toRotationMatrix();
    //     assert(R1.isApprox(R11));

    //     auto q3 = q2 - q1;
    //     auto q4 = q1.slerp(0.5, q2);
    //     auto q44 = q1 + 0.5 * q3;
    //     assert(q4.isApprox(q44));

    //     auto tt = Eigen::VectorXd::LinSpaced(100, 0, 1);
    //     for (auto t : tt) {
    //         assert((q1.slerp(t, q2)).isApprox(q1 + t * (q2 - q1)));
    //     }

    //     auto q5 = q1.conjugate() * q2;

    //     const auto R5 = q5.toRotationMatrix();
    //     IC(q5.coeffs().transpose());
    //     IC(q5.norm());
    //     assert((q1 * q5).isApprox(q2));
    //     assert((R1 * R5).isApprox(R2));

    //     const auto R12 = R1.inverse() * R2;
    //     IC(R12);
    //     assert(R12.isApprox(R5));

    //     const auto angle1 = q1.angularDistance(q2);
    //     const auto angle2 = q2.angularDistance(q1);
    //     IC(angle1);
    //     IC(angle2);
    //     assert(std::abs(angle1 - angle2) < eps);

    //     Eigen::AngleAxisd aa(R12);
    //     const auto angle12 = aa.angle();
    //     const auto axis12 = aa.axis();
    //     IC(angle12);
    //     IC(axis12.transpose());
    //     assert(std::abs(angle12 - angle2) < eps);
    //     auto axis = q5.vec().normalized();
    //     IC(axis.transpose());
    //     IC(q5.w());

    //     assert(q3.isApprox(angle12 * axis12));

    //     const auto q6 = q1.slerp(0.618, q2);
    //     const auto R6 = q6.toRotationMatrix();
    //     Eigen::AngleAxisd aa126(angle12 * 0.618, axis12);
    //     const auto R66 = R1 * aa126.toRotationMatrix();
    //     assert(R6.isApprox(R66));
    // }
}