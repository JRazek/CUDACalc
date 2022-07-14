#pragma once

#include "function.hpp"
#include <concepts>
#include <type_traits>
#include <array>
#include <concepts.hpp>
#include <utility>


namespace cuda::calc{


template<
	RealFunction Function,
	Arithmetic T
>
auto riemann_integral(
		Function function, 
		std::array<std::pair<T, T>, params_counter::FunctionArgsCount<Function>> const& ranges,
		std::array<T, params_counter::FunctionArgsCount<Function>> const& deltas
) ->  decltype(function(deltas)) {
	
	return 0;
}

}

