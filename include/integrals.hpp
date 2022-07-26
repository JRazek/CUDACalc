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
#include <ranges>

#include "integrals_cuda.hpp"



namespace jr::calc{

namespace detailed{

template<typename T, typename Function>
using ParamSizedArray = std::array<T, params_counter::ArraySizeFromCallable<Function>>;

auto inline positive_range(std::ranges::range auto const& range) -> bool {
	for(auto const& e : range)
		if(e < 0) return false;
	return true;
}

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
		Function const& function, 
		std::array<std::pair<T, T>, Nm> ranges,
		std::array<T, Nm> const& deltas
) -> T {
	assert(detailed::positive_range(deltas));

	using SizesArray=std::array<std::size_t, Nm>;

	SizesArray dims;
	bool sign;

	detailed::prepare_dims(ranges, deltas, dims, sign);

	auto result=T();

	nested_for_loop(
		dims,
		[&result, &function, &deltas, &ranges](SizesArray const& index_pack){
			std::array<T, Nm> point;

			for(auto i=0u;i<Nm;i++)
				point[i] = index_pack[i] * deltas[i] + ranges[i].first;

			result += function(point);
		}
	);

	result *= std::accumulate(deltas.begin(), deltas.end(), T(sign ? -1 : 1), std::multiplies<T>());

	return result;
}

/**
* @brief calculates approximated value of riemann integral
*
* @tparam Function - type of function to integrate
* @tparam T - return type of integral
* @param function - function to integrate
* @param ranges - ranges of integration
* @param deltas - deltas for integration (only positive values accepted)
*
* @note ranges of integration are in relative order to deltas.
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
		std::array<std::pair<T, T>, Nm> ranges,
		std::array<T, Nm> const& deltas
) -> T {
	assert(detailed::positive_range(deltas));
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
	return {};
}




}

