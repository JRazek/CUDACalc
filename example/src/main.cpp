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

auto main() -> int { 
	constexpr auto function1=[](std::array<double, 2> const& x) -> double { 
		return std::sin(x[0]) + std::cos(x[1]);
	};

	auto zero_pi_range=std::pair(0., std::numbers::pi_v<double>);
	std::array ranges{zero_pi_range, zero_pi_range};

	std::array deltas{.001, .001};

	auto res_cuda=jr::calc::riemann_integral<jr::calc::CalculationMode::cuda>(function1, ranges, deltas);
	auto res_cpu=jr::calc::riemann_integral(function1, ranges, deltas);

	std::cout<<res_cuda<<'\n';
	std::cout<<res_cpu<<'\n';
}
