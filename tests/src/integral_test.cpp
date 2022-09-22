#include <array>
#include <chrono>
#include <cmath>
#include <complex>
#include <concepts>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numbers>
#include <random>
#include <thread>
#include <vector>

#include "concepts.hpp"
#include "gtest/gtest.h"
#include "integrals.hpp"
#include "integrals_cuda.hpp"
#include "utils.hpp"

auto print_res(std::string const& worker, auto const duration,
               std::string const& duration_type = "") -> void {
  std::cout << worker << ":" << std::setw(20 - worker.size()) << duration
            << duration_type << '\n';
}

template <std::size_t N>
struct TestPack {
  std::array<double, N> deltas;
  std::array<jr::calc::range<double>, N> ranges;
};

template <std::size_t N>
auto generate_test_pack(std::size_t test_count, bool reversed_bounds = false)
    -> std::vector<TestPack<N>> {
  std::vector<TestPack<N>> test_packs(test_count);

  std::uniform_real_distribution<double> unif(0, 4);

  std::random_device rd;

  for (auto& test_pack : test_packs) {
    auto& ranges = test_pack.ranges;
    auto& deltas = test_pack.deltas;

    for (auto j = 0u; j < N; j++) {
      ranges[j] = jr::calc::range<double>{unif(rd), unif(rd)};
      if (reversed_bounds && ranges[j].low > ranges[j].high)
        std::swap(ranges[j].low, ranges[j].high);
      else if (!reversed_bounds && ranges[j].low < ranges[j].high)
        std::swap(ranges[j].low, ranges[j].high);
    }

    for (auto& d : deltas) {
      d = 0.01;
    }
  }
  return test_packs;
};

template <jr::calc::CalculationMode mode = jr::calc::CalculationMode::cpu,
          typename Function, typename Integral, std::size_t N>
auto run_test(Function const& function, Integral const& analytic_integral,
              TestPack<N> const& test_pack) -> void {
  auto& ranges = test_pack.ranges;
  auto& deltas = test_pack.deltas;

  auto res_cuda = jr::calc::riemann_integral<mode>(function, ranges, deltas);

  auto real_result = analytic_integral(ranges);
  EXPECT_NEAR(real_result, res_cuda, 0.1);
}

TEST(IntegralCudaTest2D, LinearReversedRange) {
  constexpr auto function = [](std::array<double, 1> const& x) -> double {
    return x[0];
  };

  constexpr auto analytic_integral =
      [](std::array<jr::calc::range<double>, 1> const& ranges) -> double {
    auto inverse = [](double x) { return 0.5 * x * x; };
    return inverse(ranges[0].high) - inverse(ranges[0].low);
  };

  auto test_packs = generate_test_pack<1>(10, true);

  for (auto const& test_pack : test_packs) {
    run_test<jr::calc::CalculationMode::cuda>(function, analytic_integral,
                                              test_pack);
  }
}
TEST(IntegralCudaTest2D, ConstantFunction) {
  constexpr auto function = [](std::array<double, 2> const& x) -> double {
    return 1;
  };

  constexpr auto analytic_integral =
      [](std::array<jr::calc::range<double>, 2> const& ranges) -> double {
    return (ranges[0].high - ranges[0].low) * (ranges[1].high - ranges[1].low);
  };

  auto test_packs = generate_test_pack<2>(10);

  for (auto const& test_pack : test_packs) {
    run_test<jr::calc::CalculationMode::cuda>(function, analytic_integral,
                                              test_pack);
  }
}

TEST(IntegralCudaTest2D, ComplexFunction) {
  using complex = std::complex<double>;
  using namespace jr::calc;

  constexpr auto function =
      [](std::array<double, 1> const& x) -> std::complex<double> {
    return complex{0, 1} * x[0];
  };

  auto expected_res = complex{0, 2};

  math_vec<range<double>, 1> ranges{{{0, 2}}};
  math_vec<double, 1> deltas{0.001};

  auto numerical_res = riemann_integral(function, ranges, deltas);

  auto distance_sq = std::norm(numerical_res - expected_res);

  EXPECT_NEAR(distance_sq, 0, 0.0001);
}

TEST(IntegralCudaTest2D, ZeroRange) {
  using namespace jr::calc;

  constexpr auto function = [](std::array<double, 1> const& x) -> double {
    return 5;
  };

  math_vec<range<double>, 1> ranges{{{0, 0}}};
  math_vec<double, 1> deltas{0.001};

  auto numerical_res = riemann_integral(function, ranges, deltas);

  auto expected_res = 0.;

  EXPECT_NEAR(numerical_res, expected_res, 0.0001);
}

auto main() -> int {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
