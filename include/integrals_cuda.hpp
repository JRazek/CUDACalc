#pragma once

#include <cstdio>
#include <cassert>
#include <cmath>
#include <concepts.hpp>


namespace jr::calc::cuda{

__device__
auto get_cartesian_product(){}

template<typename Function>
__global__
void kernel(Function const& function){
	auto id=threadIdx.x;

	std::printf("thread_id=%d\n", id);

	function();
}

template<
	Arithmetic T,
	RealFunction Function,
	std::size_t Nm
>
auto riemann_integral(
		Function const& function, 
		std::array<std::pair<T, T>, Nm> const& ranges,
		std::array<T, Nm> const& deltas
) -> T {

	kernel<<<1, 1>>>(function);

	return 0;
}

}

