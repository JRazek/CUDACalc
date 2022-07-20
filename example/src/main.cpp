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


	auto cuda_thread = std::async(std::launch::async, [function1, ranges, deltas]{
		auto start = std::chrono::steady_clock::now();

		auto res_cuda=jr::calc::riemann_integral<jr::calc::CalculationMode::cuda>(function1, ranges, deltas);

		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		return duration;
	});

	auto cpu_thread = std::async(std::launch::async, [function1, ranges, deltas]{
		auto start = std::chrono::steady_clock::now();

		auto res_cpu=jr::calc::riemann_integral(function1, ranges, deltas);

		auto end = std::chrono::steady_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

		return duration;
	});

	cuda_thread.wait();

	cpu_thread.wait();


	auto duration_cuda=cuda_thread.get();

	auto duration_cpu=cpu_thread.get();

	print_res("cuda", duration_cuda);

	print_res("cpu", duration_cpu);

	print_res("ratio", (double(duration_cpu)/duration_cuda));

}
