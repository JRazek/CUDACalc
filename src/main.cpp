#include "function.hpp"
#include <iostream>
#include <integrals.hpp>

auto main() -> int { 

	constexpr auto function1=[](std::array<double, 1> const& x) -> double { return 2*x[0]; };
	auto derivative=[](double x) -> double { return 2; };
	auto integral=[](double x) -> double { return x*x; };

	std::array ranges{std::pair(.0, .1)};
	std::array deltas{.1};

	auto res=cuda::calc::riemann_integral<decltype(function1), double>(function1, ranges, deltas);

}
