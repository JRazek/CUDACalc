#pragma once

#include "utils.hpp"
#include <concepts>
#include <type_traits>
#include <array>
#include <concepts.hpp>
#include <utility>
#include <cmath>


namespace jr::calc{

template<typename T, typename Function>
using ParamSizedArray = std::array<T, params_counter::FunctionArgsCount<Function>>;

/**
* @brief calculates approximated value of riemann integral
*
* @tparam Function - type of function to integrate
* @tparam T - return type of integral
* @param function - function to integrate
* @param ranges - ranges of integration
* @param deltas - deltas for integration
*
* @note ranges of integration are in relative order to deltas.
*
* @return 
*/
template<
	RealFunction Function,
	Arithmetic T,
	CalculationMode mode = CalculationMode::cpu
>
auto riemann_integral(
		Function function, 
		std::array<std::pair<T, T>, params_counter::FunctionArgsCount<Function>> const& ranges,
		ParamSizedArray<T, Function> const& deltas
) -> std::invoke_result_t<Function, ParamSizedArray<T, Function>> {

	std::size_t ker_count=1;

	for(auto i=std::size_t();i<ranges.size();i++){
		ker_count *= std::ceil((double(ranges[i].second-ranges[i].first))/deltas[i]);
	}

	if constexpr (mode==CalculationMode::cpu) {

	}
	else if constexpr (mode==CalculationMode::cuda){

	}

	return 0;
}


}

