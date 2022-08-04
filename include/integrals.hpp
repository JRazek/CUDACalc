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
#include <algorithm>

#include "integrals_cuda.hpp"



namespace jr::calc{

namespace detailed{

template<typename T, typename Function>
using ParamSizedArray = std::array<T, params_counter::ArraySizeFromCallable<Function>>;

auto inline positive_range(std::ranges::range auto const& range) -> bool {
	return std::all_of(range.begin(), range.end(), [](auto const n){ return n>=0; });
}

}

/**
* @brief calculates approximated value of riemann integral over R^n -> R^1 function
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
		math_vec<range<T>, Nm> ranges,
		math_vec<T, Nm> const& deltas
) -> T {
	assert(detailed::positive_range(deltas));

	using SizesArray=math_vec<std::size_t, Nm>;

	SizesArray dims;
	bool sign;

	detailed::prepare_dims(ranges, deltas, dims, sign);

	auto result=T();

	nested_for_loop(
		dims,
		[&result, &function, &deltas, &ranges](SizesArray const& index_pack){
			math_vec<T, Nm> point;

			for(auto i=0u;i<Nm;i++)
				point[i] = index_pack[i] * deltas[i] + ranges[i].first;

			result += function(point);
		}
	);

	result *= std::accumulate(deltas.begin(), deltas.end(), T(sign ? -1 : 1), std::multiplies<T>());

	return result;
}

/**
* @brief calculates approximated value of riemann integral over R^n -> R^1 function
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
		math_vec<range<T>, Nm> ranges,
		math_vec<T, Nm> const& deltas
) -> T {
	assert(detailed::positive_range(deltas));
	return jr::calc::cuda::riemann_integral(function, ranges, deltas);
}

/**
* @brief calculates approximated derivative of a function over R^n -> R^1 function
*
* @tparam Function - type of function to differentiate
* @tparam T - return type of function
* @param function - function to differentiate
* @param ranges - cartesian product of differentiation point ()
* @param deltas - deltas for differentiation (dx, dy, ...)
* @param gradient - result of a calculation
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
		math_vec<T, Nm> points,
		math_vec<T, Nm> const& deltas,
		math_vec<T, Nm>& gradient
) -> void {
	for(auto i=0u;i<Nm;i++){
		auto value = function(points);
		points[i] += deltas[i];
		auto high = function(points);
		gradient[i] = (high - value) / deltas[i];
	}
}

/**
* @brief calculates approximated derivative of a function over R^n -> R^1 function
*
* @tparam Function - type of function to differentiate
* @tparam T - return type of function
* @param function - function to differentiate
* @param ranges - cartesian product of differentiation point ()
* @param deltas - deltas for differentiation (dx, dy, ...)
*
* @note ranges of integration are in relative order to deltas.
*
* @return device pointer to a sequence of size Nm 
*/
template<
	CalculationMode mode = jr::calc::CalculationMode::cpu,
	std::size_t kBlockSize = 64,
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
requires ( mode == jr::calc::CalculationMode::cuda )
auto calculate_gradient(
		Function const& function, 
		math_vec<T, Nm> const& points,
		math_vec<T, Nm> const& deltas
) -> thrust::device_vector<T> {
	return jr::calc::cuda::calculate_gradient(function, points, deltas);
}

}

