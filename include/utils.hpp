#pragma once

#include <array>
#include <concepts.hpp>
#include <cstddef>
#include <concepts>
#include <utility>
#include <iostream>

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
	typename Runnable,
	typename... Args
>
requires std::invocable<Runnable, std::array<std::size_t, Nm>, Args...>
auto nested_for_loop(std::array<std::size_t, Nm> const& dims, Runnable const& runnable, Args&&... args) -> void {
	auto accumulated_products=dims;
	for(auto i=1u;i<Nm;i++) accumulated_products[i]*=accumulated_products[i-1];

	auto n=accumulated_products.back();

	for(auto i=0u;i<n;i++){
		std::array<std::size_t, Nm> index_pack{i%accumulated_products[0]};

		for(auto d=1u;d<Nm;d++)
			index_pack[d] = i % accumulated_products[d] / accumulated_products[d-1];

		runnable(index_pack, std::forward<Args>(args)...);
	}
}

enum class CalculationMode{
	cpu,
	cuda,
};


}
