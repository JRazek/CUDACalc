#include "function.hpp"
#include <iostream>
#include <integrals.hpp>

auto main() -> int { 

	cuda::calc::riemann_integral(cuda::calc::R_RFunction{}, 2);

	auto test=cuda::calc::R_RFunction::dim_value;
}
