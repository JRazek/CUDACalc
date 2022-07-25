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


namespace jr::calc{

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
requires std::invocable<Runnable, std::array<std::size_t, Nm>>
auto nested_for_loop(std::array<std::size_t, Nm> const& dims, Runnable const& runnable) -> void {
	auto accumulated_products=dims;

	std::partial_sum(accumulated_products.begin(), accumulated_products.end(), accumulated_products.begin(), std::multiplies<std::size_t>());

	auto n=accumulated_products.back();

	for(auto i=0u;i<n;i++){
		std::array<std::size_t, Nm> index_pack{i%accumulated_products[0]};

		for(auto d=1u;d<Nm;d++)
			index_pack[d] = i % accumulated_products[d] / accumulated_products[d-1];

		runnable(index_pack);
	}
}

enum class CalculationMode{
	cpu,
	cuda,
};


namespace cuda{

template<
	typename T
>
__device__
auto copy(const T* input, T* output, std::size_t size) -> void{
	memcpy(static_cast<void*>(output), static_cast<const void*>(input), size*sizeof(T));
}

}


}
