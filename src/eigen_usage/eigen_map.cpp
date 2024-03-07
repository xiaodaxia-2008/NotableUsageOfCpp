#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <numeric>
#include <string_view>

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

struct S
{
    float x;
    float y;
    float z;
    char name[100];
};

float f(int v) { return v + 0.1; }

int f(float v) { return v - 1; }

int main()
{
    using std::operator""sv;

    std::vector<int> nums(10);
    std::iota(nums.begin(), nums.end(), 0);
    spdlog::info("nums: {}", nums);

    std::vector<float> fnums(10);
    std::transform(nums.begin(), nums.end(), fnums.begin(),
                   static_cast<float (*)(int)>(&f));
    spdlog::info("fnums: {}", fnums);

    constexpr auto s1 = "this is a string view"sv;

    std::vector<S> ss;
    ss.push_back(S{1.f, 2.f, 3.f, "first"});
    ss.push_back(S{1.1f, 2.1f, 3.1f, "second"});
    ss.push_back(S{1.2f, 2.2f, 3.2f, "third"});
    ss.push_back(S{1.3f, 2.3f, 3.3f, "fouth"});
    ss.push_back(S{1.4f, 2.4f, 3.4f, "fithth"});
    ss.push_back(S{1.5f, 2.5f, 3.5f, "sixth"});

    using namespace Eigen;

    constexpr auto outer_stride = 3;
    constexpr auto inner_stride = sizeof(S) / sizeof(float);
    Eigen::Map<MatrixXf, 0, Eigen::Stride<1, inner_stride>> m(
        &ss[0].x, ss.size(), outer_stride);

    std::cout << m << std::endl;
    return 0;
}