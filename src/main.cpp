#include <iostream>
#include "integrals.hpp"
#include "utils.hpp"
#include <array>
#include <memory>


auto main() -> int { 

	constexpr auto function1=[](std::array<double, 1> const& x) -> double { 
		std::cout<<"x="<<x[0]<<'\n';
		return x[0];
	};
//	auto derivative=[](double x) -> double { return 2; };
//	auto integral=[](double x) -> double { return x*x; };

	std::array ranges{std::pair(0., 1.)};
	std::array deltas{.05};

	auto res=jr::calc::riemann_integral<decltype(function1), double, jr::calc::CalculationMode::cpu>(function1, ranges, deltas);


	std::cout<<res<<'\n';
}
