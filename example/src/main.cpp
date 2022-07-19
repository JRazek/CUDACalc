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


auto print_res(std::string const& worker, auto const duration) -> void{
	std::cout<<worker<<":"<<std::setw(20-worker.size())<<duration<<'\n';
}

auto main() -> int { 
	constexpr auto function1=[](std::array<double, 2> const& x) -> double { 
		return std::sin(x[0]) + std::cos(x[1]);
	};

	auto zero_pi_range=std::pair(0., 8*std::numbers::pi_v<double>);
	std::array ranges{zero_pi_range, zero_pi_range};

	std::array deltas{.001, .001};


	auto cuda_thread = std::thread([function1, ranges, deltas]{
		auto start = std::chrono::steady_clock::now();

		auto res_cuda=jr::calc::riemann_integral<jr::calc::CalculationMode::cuda>(function1, ranges, deltas);

		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		print_res("cuda", duration);
	});

	auto cpu_thread = std::thread([function1, ranges, deltas]{
		auto start = std::chrono::steady_clock::now();

		auto res_cpu=jr::calc::riemann_integral(function1, ranges, deltas);

		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		print_res("cpu", duration);
	});

	cuda_thread.join();

	cpu_thread.join();
}
