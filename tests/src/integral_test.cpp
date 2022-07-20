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
			
			for(auto& [low, high] : ranges){
				if(low>high)
					std::swap(low, high);
			}

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
	constexpr auto function=[](std::array<double, 2> const& x) -> double { 
		return std::sin(x[0]) + std::cos(x[1]);
	};

	constexpr auto analytic_integral = [](std::array<std::pair<double, double>, 2> const& ranges) -> double { 
		return (ranges[0].second - ranges[0].first) * (ranges[1].second - ranges[1].first);
	};

	for(auto const& test_pack : test_packs){
		run_test(function, analytic_integral, test_pack);
	}
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



auto main() -> int { 
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
