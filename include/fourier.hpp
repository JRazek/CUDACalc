#pragma once

#include <concepts.hpp>
#include <complex>
#include <tuple>


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
		std::array<std::pair<T, T>, Nm> const& ranges,
) -> std::array<std::complex<T>, Nm> {
	
}


}
