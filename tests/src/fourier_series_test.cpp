#include <cmath>
#include <complex>
#include <numbers>
#include <random>

#include "fourier.hpp"
#include "gtest/gtest.h"
#include "integrals.hpp"
#include "utils.hpp"

using namespace jr::calc;

TEST(FourierSeries, RealFunctionSin) {
  using complex = std::complex<double>;

  auto scalar_field = [](math_vec<double, 1> const& x) {
    return std::sin(x[0]);
  };

  using FourierSeries = jr::calc::FourierSeries<101, complex, double>;

  range<double> period{0., 2 * std::numbers::pi};

  auto fourier_series =
      FourierSeries::calculate_fourier_series<CalculationMode::cpu>(scalar_field, period, 0.001);

  int samples_count = 100;

  std::mt19937 rng;
  std::uniform_real_distribution<double> gen(period.low, period.high);

  for (auto i = 0u; i < samples_count; i++) {
    auto n = gen(rng);

    math_vec<double, 1> x{n};
    auto expected_res = scalar_field(x);
    auto numerical_res = fourier_series(x);

//    std::cerr << "expected_res: " << expected_res << '\n';
//    std::cerr << "numerical_res: " << numerical_res << '\n';

    EXPECT_NEAR(std::norm(expected_res - numerical_res), 0, 0.0001);
  }
}

TEST(FourierSeries, ComplexSin) {
  using complex = std::complex<double>;

  auto scalar_field = [](math_vec<double, 1> const& x) {
    return complex{std::sin(x[0]), std::sin(x[0])};
  };

  using FourierSeries = jr::calc::FourierSeries<101, complex, double>;

  range<double> period{0., 2 * std::numbers::pi};

  auto fourier_series =
      FourierSeries::calculate_fourier_series<CalculationMode::cpu>(scalar_field, period, 0.001);

  int samples_count = 100;

  std::mt19937 rng;
  std::uniform_real_distribution<double> gen(period.low, period.high);

  for (auto i = 0u; i < samples_count; i++) {
    auto n = gen(rng);

    math_vec<double, 1> x{n};
    auto expected_res = scalar_field(x);
    auto numerical_res = fourier_series(x);

//    std::cerr << "expected_res: " << expected_res << '\n';
//    std::cerr << "numerical_res: " << numerical_res << '\n';

    EXPECT_NEAR(std::norm(expected_res - numerical_res), 0, 0.0001);
  }
}

auto main() -> int {
  ::testing::InitGoogleTest();

  return RUN_ALL_TESTS();
}
