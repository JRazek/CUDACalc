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
	constexpr auto function1=[]__device__(std::array<double, 2> const& x) -> double { 
		return std::sin(x[0]) + std::cos(x[1]);
	};
}
