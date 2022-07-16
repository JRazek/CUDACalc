#pragma once

#include "utils.hpp"
#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <array>
#include <concepts.hpp>
#include <utility>
#include <cmath>
#include <iostream>
#include <numeric>
#include <cassert>


namespace jr::calc{

namespace detailed{

template<typename T, typename Function>
using ParamSizedArray = std::array<T, params_counter::ArraySizeFromCallable<Function>>;

}

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
	CalculationMode mode = CalculationMode::cpu,
	Arithmetic T,
	RealFunction Function
>
auto riemann_integral(
		Function function, 
		detailed::ParamSizedArray<std::pair<T, T>, Function> const& ranges,
		detailed::ParamSizedArray<T, Function> const& deltas
) -> std::invoke_result_t<Function, detailed::ParamSizedArray<T, Function>> {
	auto result=T();

	constexpr auto N = params_counter::ArraySizeFromCallable<Function>;

	using SizesArray=std::array<std::size_t, N>;

	SizesArray dims;

	for(auto i=std::size_t();i<ranges.size();i++)
		dims[i] = std::ceil((double(ranges[i].second-ranges[i].first))/deltas[i]);

	if constexpr (mode==CalculationMode::cpu) {
		nested_for_loop(
			dims,
			[&result, &function, &deltas, N](SizesArray const& index_pack){
				std::array<T, N> point;

				for(auto i=0u;i<N;i++)
					point[i] = index_pack[i] * deltas[i];

				result += function(point);
			}
		);

		result *= std::accumulate(deltas.begin(), deltas.end(), T(1), std::multiplies<T>());
	}
	else if constexpr (mode==CalculationMode::cuda) {
		assert(false);
	}

	return result;
}


}

