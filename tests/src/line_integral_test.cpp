#include <cmath>
#include <complex>
#include <numbers>

#include "gtest/gtest.h"
#include "integrals.hpp"
#include "utils.hpp"

using namespace jr::calc;

TEST(LineIntegral, StraightLine) {
  auto line = [](math_vec<double, 1> const& x) { return x; };

  auto integral = [](range<double> const& range) {
    return 4 * (range.high - range.low);
  };

  auto scalar_field = [](math_vec<double, 1> const& x) { return 4.; };

  double delta = 0.01;

  range<double> range{-1., 1.};

  auto numerical_res =
      calculate_line_integral(scalar_field, range, line, delta);

  EXPECT_NEAR(integral(range), numerical_res, 0.1);
}

TEST(LineIntegral, WaveFunction) {
  auto circle = [](math_vec<double, 1> const& x) {
    return math_vec<double, 2>{std::cos(x[0]), std::sin(x[0])};
  };

  auto scalar_field = [](math_vec<double, 2> const& x) {
    return std::cos(x[0]) * std::cos(x[1]);
  };

  double delta = 0.01;

  range<double> range{0., 2 * std::numbers::pi};

  auto expected_res = 3.51314;

  auto numerical_res =
      calculate_line_integral(scalar_field, range, circle, delta);

  EXPECT_NEAR(expected_res, numerical_res, 0.01);
}

TEST(LineIntegral, ComplexFunction) {
  using complex = std::complex<double>;

  auto curve = [](math_vec<double, 1> const& t) {
    return math_vec<complex, 1>{complex{0., 1.} * t[0]};
  };

  auto scalar_field = [](math_vec<complex, 1> const& x) {
    return std::exp(x[0]);
  };

  double delta = 0.001;

  range<double> range{0., 0.5*std::numbers::pi};

  auto expected_res = complex{-1, 1};

  auto numerical_res =
      calculate_line_integral(scalar_field, range, curve, delta);

  auto distance_sq = std::norm(numerical_res-expected_res);

  EXPECT_NEAR(distance_sq, 0, 0.00001);
}

auto main() -> int {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
