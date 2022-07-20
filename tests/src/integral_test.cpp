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


auto print_res(std::string const& worker, auto const duration, std::string const& duration_type="") -> void{
	std::cout<<worker<<":"<<std::setw(20-worker.size())<<duration<<duration_type<<'\n';
}


TEST(IntegralCudaTest, ConstantFunction) {
	constexpr auto function1=[](std::array<double, 2> const& x) -> double { 
		return 1;
	};

	constexpr auto analytic_integral = [](std::array<std::pair<double, double>, 2> const& ranges) -> double { 
		return (ranges[0].second-ranges[0].first)*(ranges[1].second-ranges[1].first);
	};

	std::uniform_real_distribution<double> unif(-10, 10);
	std::default_random_engine re;
	
	for(auto i=0;i<10;i++){
		std::array ranges{std::pair(unif(re), unif(re)), std::pair(unif(re), unif(re))};
		std::array deltas{unif(re), unif(re)};

		auto res_cuda=jr::calc::riemann_integral<jr::calc::CalculationMode::cuda>(function1, ranges, deltas);
		auto real_result=analytic_integral(ranges);
		ASSERT_FLOAT_EQ(real_result, res_cuda);
	}
}



auto main() -> int { 
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
