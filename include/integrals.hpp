#pragma once

#include "function.hpp"
#include <concepts>
#include <type_traits>
#include <array>
#include <concepts.hpp>


namespace cuda::calc{

	template<
		RealFunction Function,
		Arithmetic T
	>
	auto riemann_integral(Function function, std::array<T, Function::dim_value> const& deltas){
		
	}

	template<
		typename Function,
		typename... Deltas	
	>
	auto riemann_integral(Function function, Deltas... deltas_pack)
	requires RealFunctionParams<Function, Deltas...> {
		std::array deltas{deltas_pack...};
		return riemann_integral(function, deltas);
	}

}
