#pragma once

#include <array>
#include <concepts.hpp>
#include <cstddef>
#include <concepts>
#include <utility>
#include <iostream>
#include <ranges>
#include <iterator>
#include <cuda_runtime_api.h>
#include <numeric>
#include <functional>


namespace jr::calc{

enum class CalculationMode{
	cpu,
	cuda,
};

template <typename T>
struct range{
	T low, high;
};

template <typename Input>
auto euclidean_norm(Input const& vec) {
	using T = typename Input::value_type;
	return std::sqrt(
		std::reduce(vec.begin(), vec.end(), T(0), 
		[](auto const res, auto const n){
			return res + n*n;
		})
	);
}

template<
	typename T,
	std::size_t N
>
using math_vec = std::array<T, N>;
namespace detailed{

template<typename T, std::size_t N>
inline auto prepare_dims(
		math_vec<range<T>, N>& ranges,
		math_vec<T, N> const& deltas,
		math_vec<std::size_t, N>& dims,
		bool& sign
		) -> void {
	sign = false;

	for(auto i=std::size_t();i<ranges.size();i++){
		if(ranges[i].low > ranges[i].high){ 
			std::swap(ranges[i].low, ranges[i].high); 
			sign = !sign;
		}

		auto range_size = ranges[i].high - ranges[i].low;

		dims[i] = std::ceil(range_size/deltas[i]);
	}
}

}

/**
* @brief - provides interface for nested for loops
*
* @tparam Nm - number of dimensions (nested for loops)
* @tparam Runnable - runnable type to be invoked
* @tparam Args - type of user data
* @param runnable - runnable
* @param args - user data
*/
template <
	std::size_t Nm,
	typename Runnable
>
requires std::invocable<Runnable, math_vec<std::size_t, Nm>>
auto nested_for_loop(math_vec<std::size_t, Nm> const& dims, Runnable const& runnable) -> void {
	auto accumulated_products=dims;

	std::partial_sum(accumulated_products.begin(), accumulated_products.end(), accumulated_products.begin(), std::multiplies<std::size_t>());

	auto n=accumulated_products.back();

	for(auto i=0u;i<n;i++){
		math_vec<std::size_t, Nm> index_pack{i%accumulated_products[0]};

		for(auto d=1u;d<Nm;d++)
			index_pack[d] = i % accumulated_products[d] / accumulated_products[d-1];

		runnable(index_pack);
	}
}

template <typename T>
auto operator<<(std::ostream& stream, range<T> const& range) -> std::ostream{
	return stream<<"{low: "<<range.low<<", high: "<<range.high<<"}";
}

namespace cuda{

template<
	typename T
>
__device__
auto copy(const T* input, T* output, std::size_t size) -> void{
	memcpy(static_cast<void*>(output), static_cast<const void*>(input), size*sizeof(T));
}

}

template <
	ScalarType Scalar,
	std::size_t N
>
auto operator+=(math_vec<Scalar, N>& lhs, math_vec<Scalar, N>& rhs) -> math_vec<Scalar, N>& {
	std::transform(
			std::begin(lhs),
			std::end(lhs),
			std::begin(rhs),
			std::begin(lhs),
			std::plus<Scalar>()
	);
	return lhs;
}

template <
	ScalarType Scalar,
	std::size_t N
>
auto operator-=(math_vec<Scalar, N>& lhs, math_vec<Scalar, N>& rhs) -> math_vec<Scalar, N>& {
	std::transform(
			std::begin(lhs),
			std::end(lhs),
			std::begin(rhs),
			std::begin(lhs),
			std::minus<Scalar>()
	);
	return lhs;
}

}
