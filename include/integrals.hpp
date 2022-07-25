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

#include "integrals_cuda.hpp"


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
* range must be a valid range where the first param is lower or equal to the second one.
* Otherwise behaviour of the program is undefined.
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
auto riemann_integral(
		Function function, 
		std::array<std::pair<T, T>, Nm> ranges,
		std::array<T, Nm> deltas
) -> T {
	using SizesArray=std::array<std::size_t, Nm>;

	SizesArray dims;
	bool sign = false;

	for(auto i=std::size_t();i<ranges.size();i++){
		if(ranges[i].first > ranges[i].second){ 
			std::swap(ranges[i].first, ranges[i].second); 
			sign = !sign;
		}

		if constexpr(std::is_unsigned_v<T>){
			if(deltas[i] < 0){
				sign = !sign;
				deltas[i] *= -1;
			}
		}

		auto range_size = ranges[i].second - ranges[i].first;

		dims[i] = std::ceil(range_size/deltas[i]);
	}

	auto result=T();

	nested_for_loop(
		dims,
		[&result, &function, &deltas, &ranges, sign = (sign ? -1 : 1)](SizesArray const& index_pack){
			std::array<T, Nm> point;

			for(auto i=0u;i<Nm;i++)
				point[i] = index_pack[i] * deltas[i] + ranges[i].first;

			result += function(point) * sign;
		}
	);

	result *= std::accumulate(deltas.begin(), deltas.end(), T(1), std::multiplies<T>());

	return result;
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
* range must be a valid range where the first param is lower or equal to the second one.
* Otherwise behaviour of the program is undefined.
*
* @return 
*/
template<
	CalculationMode mode,
	std::size_t kBlockSize = 64,
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
requires 
(mode == CalculationMode::cuda)
auto riemann_integral(
		Function function, 
		std::array<std::pair<T, T>, Nm> const& ranges,
		std::array<T, Nm> const& deltas
) -> T {
	return jr::calc::cuda::riemann_integral(function, ranges, deltas);
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
	CalculationMode mode = jr::calc::CalculationMode::cpu,
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
requires ( mode == jr::calc::CalculationMode::cpu )
auto calculate_gradient(
		Function function, 
		std::array<T, Nm> const& point,
		std::array<T, Nm> const& deltas
) -> T {
	
}




}

