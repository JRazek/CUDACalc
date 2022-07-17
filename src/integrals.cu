#include <thrust/device_ptr.h>
#include <thrust/device_vector.h>
#include <thrust/iterator/zip_iterator.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <cassert>

#include "integrals.cuh"
#include "cuda_api.hpp"


namespace jr::calc::cuda{

template<typename Function>
__global__
void kernel(Function const& function){
	//function();
}

auto riemann_integral(
		cuda_function_wrapper const& wrapper,
		std::vector<thrust::pair<double, double>> const& ranges, 
		std::vector<double> const& deltas
) -> double{

	assert(ranges.size() == deltas.size());

	
	thrust::device_vector<thrust::pair<double, double>> ranges_dev(ranges.begin(), ranges.end());
	thrust::device_vector<double> deltas_dev(deltas.begin(), deltas.end());


	auto zipped = thrust::make_zip_iterator(thrust::make_tuple(ranges_dev.begin(), deltas_dev.begin()));



	return 0;

}

}
