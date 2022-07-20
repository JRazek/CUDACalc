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



auto print_res(std::string const& worker, auto const duration, std::string const& duration_type="") -> void{
	std::cout<<worker<<":"<<std::setw(20-worker.size())<<duration<<duration_type<<'\n';
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

		return std::pair(res_cuda, duration);
	});

//	auto cpu_thread = std::async(std::launch::async, [function1, ranges, deltas]{
//		auto start = std::chrono::steady_clock::now();
//
//		auto res_cpu=jr::calc::riemann_integral(function1, ranges, deltas);
//
//		auto end = std::chrono::steady_clock::now();
//
//		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
//
//		return std::pair(res_cpu, duration);
//	});

	cuda_thread.wait();

//	cpu_thread.wait();


	auto [res_cuda, duration_cuda]=cuda_thread.get();

//	auto [res_cpu, duration_cpu]=cpu_thread.get();

	print_res("cuda", duration_cuda, "ns");

//	print_res("cpu", duration_cpu, "ns");

//	print_res("speedup", std::to_string((double(duration_cpu)/duration_cuda))+"x");

	std::cout<<"res_cuda="<<res_cuda<<'\n';
}
