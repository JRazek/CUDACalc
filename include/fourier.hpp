#pragma once

#include <concepts.hpp>
#include <complex>
#include <tuple>

#include <utils.hpp>

namespace jr::calc{
	
template<
	CalculationMode mode = CalculationMode::cpu,
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
requires 
(mode == CalculationMode::cpu && params_counter::ArraySizeFromCallable<Function> == Nm)
	and
(Nm == 1)
auto fourier_series(
		Function function, 
		math_vec<std::pair<T, T>, Nm> const& ranges,
) -> math_vec<std::complex<T>, Nm> {
	
}


}
