#pragma once
#include "integrals.cuh"
#include <vector>
#include <utility>
#include <thrust/pair.h>

namespace jr::calc::cuda{

auto riemann_integral(
		cuda_function_wrapper const& wrapper,
		std::vector<thrust::pair<double, double>> const& ranges, 
		std::vector<double> const& deltas
) -> double;


template<
	std::size_t Nm
>
static auto riemann_integral(
		cuda_function_wrapper const& wrapper,
		std::array<std::pair<double, double>, Nm> const& ranges,
		std::array<double, Nm> const& deltas
) -> double{
	std::vector<thrust::pair<double, double>> ranges_vec;

	std::transform(
		ranges.begin(), 
		ranges.end(), 
		std::back_inserter(ranges_vec),
		[](auto const std_pair){
			return thrust::pair(std_pair.first, std_pair.second);
		}
	);

	std::vector<double> deltas_vec(deltas.begin(), deltas.end());
	
	return riemann_integral(wrapper, ranges_vec, deltas_vec);
}

}
