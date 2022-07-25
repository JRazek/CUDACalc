#include "gtest/gtest.h"

#include <iostream>
#include "utils.hpp"
#include "concepts.hpp"
#include "integrals.hpp"
#include "integrals_cuda.hpp"
#include <array>
#include <memory>
#include <cmath>
#include <numbers>
#include <concepts>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <future>
#include <random>
#include <concepts>
#include <complex>


auto print_res(std::string const& worker, auto const duration, std::string const& duration_type="") -> void{
	std::cout<<worker<<":"<<std::setw(20-worker.size())<<duration<<duration_type<<'\n';
}

template<std::size_t N>
struct TestPack{
	std::array<double, N> deltas;
	std::array<std::pair<double, double>, N> ranges;
};

struct IntegralCudaTest2D : public testing::Test{

	constexpr static auto N = 2u;

	std::array<TestPack<N>, 10> test_packs;

	IntegralCudaTest2D(){
		std::uniform_real_distribution<double> unif(0, 4);

		std::random_device rd;
		
		for(auto& test_pack : test_packs){
			auto& ranges=test_pack.ranges;
			auto& deltas=test_pack.deltas;

			for(auto j=0u;j<N;j++)
				ranges[j] = std::pair(unif(rd), unif(rd));

			for(auto& d : deltas){
				d = 0.01;
			}
		}
	}
};

template<
	jr::calc::CalculationMode mode = jr::calc::CalculationMode::cpu,
	typename Function,
	typename Integral,
	std::size_t N
>
auto run_test(Function const& function, Integral const& analytic_integral, TestPack<N> const& test_pack) -> void {
	auto& ranges=test_pack.ranges;
	auto& deltas=test_pack.deltas;

	auto res_cuda=jr::calc::riemann_integral<mode>(function, ranges, deltas);

	auto real_result=analytic_integral(ranges);
	EXPECT_NEAR(real_result, res_cuda, 0.1);
}



//TODO
TEST_F(IntegralCudaTest2D, SinCosProd) { 
//	constexpr auto function=[](std::array<double, 2> const& x) -> double { 
//		return std::sin(x[0]) + std::cos(x[1]);
//	};
//
//	constexpr auto analytic_integral = [](std::array<std::pair<double, double>, 2> const& ranges) -> double { 
//		return (ranges[0].second - ranges[0].first) * (ranges[1].second - ranges[1].first);
//	};

}

TEST_F(IntegralCudaTest2D, LinearReversedRange) { 
	constexpr auto function=[](std::array<double, 1> const& x) -> double { 
		return x[0];
	};

	constexpr auto analytic_integral = [](std::array<std::pair<double, double>, 1> const& ranges) -> double { 
		auto inverse = [](double x){ return 0.5 * x * x; };
		return inverse(ranges[0].second) - inverse(ranges[0].first);
	};

	std::array ranges{std::pair(10., 2.)};
	std::array deltas{0.01};

	auto res_cuda=jr::calc::riemann_integral<jr::calc::CalculationMode::cpu>(function, ranges, deltas);
	auto real_result=analytic_integral(ranges);
	EXPECT_NEAR(real_result, res_cuda, 0.2);

}
TEST_F(IntegralCudaTest2D, ConstantFunction) {
	constexpr auto function=[](std::array<double, 2> const& x) -> double { 
		return 1;
	};

	constexpr auto analytic_integral = [](std::array<std::pair<double, double>, 2> const& ranges) -> double { 
		return (ranges[0].second - ranges[0].first) * (ranges[1].second - ranges[1].first);
	};

	for(auto const& test_pack : test_packs){
		run_test(function, analytic_integral, test_pack);
	}
}

TEST_F(IntegralCudaTest2D, ComplexFunction) {
//	constexpr auto function=[](std::array<std::complex<double>, 1> const& x) -> std::complex<double> { 
//		return x[0];
//	};
//
//	std::array arr{ std::pair(std::complex{1., 0.}, std::complex{1., 0.}) };

//	jr::calc::riemann_integral(function, arr, std::complex{1, 0});

}


auto main() -> int { 
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
