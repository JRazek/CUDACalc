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
	RealFunction Function,
	Arithmetic T,
	CalculationMode mode = CalculationMode::cpu
>
auto riemann_integral(
		Function function, 
		detailed::ParamSizedArray<std::pair<T, T>, Function> const& ranges,
		detailed::ParamSizedArray<T, Function> const& deltas
) -> std::invoke_result_t<Function, detailed::ParamSizedArray<T, Function>> {


	auto result=T();

	constexpr auto N = params_counter::ArraySizeFromCallable<Function>;

	std::size_t ker_count=1;

	using SizesArray=std::array<std::size_t, N>;

	SizesArray sizes;

	for(auto i=std::size_t();i<ranges.size();i++){
		sizes[i] = std::ceil((double(ranges[i].second-ranges[i].first))/deltas[i]);
		ker_count *= sizes[i];
	}

	if constexpr (mode==CalculationMode::cpu) {
		auto calculate_accumulated_product = [](auto arr){
			for(auto i=1u;i<arr.size();i++) 
				arr[i] *= arr[i-1];

			return arr;
		};
		
		auto accumulated_product=calculate_accumulated_product(sizes);

		nested_for_loop(
			accumulated_product, 
			[&result, &function, &deltas, N](SizesArray const& index_pack){
				std::array<T, N> point;

				for(auto i=0u;i<N;i++)
					point[i] = index_pack[i] * deltas[i];

				result += function(point);
			}
		);

//		result *= std::accumulate(deltas.begin(), deltas.end(), T(1), std::multiplies<T>());
	}
	else if constexpr (mode==CalculationMode::cuda) {
		
	}

	return result;
}


}

