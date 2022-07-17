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

#include "cuda_api.hpp"
#include "user_kernels.hpp"


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
	RealFunction Function,
	std::size_t Nm
>
requires 
(mode == CalculationMode::cpu && params_counter::ArraySizeFromCallable<Function> == Nm)
	or
(mode == CalculationMode::cuda && std::is_same_v<T, double>)
auto riemann_integral(
		Function function, 
		std::array<std::pair<T, T>, Nm> const& ranges,
		std::array<T, Nm> const& deltas
) -> T {
	auto result=T();

	constexpr auto N = Nm;

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
		jr::calc::cuda::riemann_integral(function, ranges, deltas);
	}

	return result;
}


/**
* @brief calculates approximated derivative of a function
*
* @tparam Function - type of function to differentiate
* @tparam T - return type of function
* @param function - function to differentiate
* @param ranges - cartesian product of differentiation point ()
* @param deltas - deltas for differentiation (dx, dy, ...)
*
* @note ranges of integration are in relative order to deltas.
*
* @return 
*/
template<
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
auto calculate_gradient(
		Function function, 
		std::array<T, Nm> const& point,
		std::array<T, Nm> const& deltas
) -> T {
	assert(false);
}




}

