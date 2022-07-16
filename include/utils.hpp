#pragma once

#include <array>
#include <concepts.hpp>
#include <cstddef>
#include <concepts>
#include <utility>
#include <iostream>

namespace jr::calc{


namespace detail{

template <
	std::size_t Nm,
	std::size_t kDepth,
	typename Runnable,
	typename... Args
>
auto nested_for_loop_helper(std::array<std::size_t, Nm> const& dims, std::array<std::size_t, Nm>& index_pack, Runnable const& runnable, Args&&... args) -> void {
	for(auto i=0u;i<dims[kDepth];i++){
		index_pack[kDepth]=i;

		if constexpr (kDepth<Nm-1)
			nested_for_loop_helper<Nm, kDepth+1>(dims, index_pack, runnable, std::forward<Args>(args)...);
		else
			runnable(index_pack, args...);
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
	typename Runnable,
	typename... Args
>
requires std::invocable<Runnable, std::array<std::size_t, Nm>, Args...>
auto nested_for_loop(std::array<std::size_t, Nm> const& dims, Runnable const& runnable, Args&&... args) -> void {
	std::array<std::size_t, Nm> index_pack{0};

	detail::nested_for_loop_helper<Nm, 0>(dims, index_pack, runnable, std::forward<Args>(args)...);

}


enum class CalculationMode{
	cpu,
	cuda,
};


}
