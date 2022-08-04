#pragma once

#include <cstdio>
#include <cassert>
#include <cmath>
#include <thrust/device_ptr.h>
#include <thrust/device_vector.h>
#include <numeric>
#include <concepts.hpp>
#include "utils.hpp"

namespace jr::calc::cuda{

template<
	std::size_t Nm
>
__device__
auto get_index_pack(
		math_vec<std::size_t, Nm> const& accumulated_products, 
		std::size_t const index,
		math_vec<std::size_t, Nm>& index_pack) -> void {
	index_pack[0] = index % accumulated_products[0];

	for(auto d=1u;d<Nm;d++)
		index_pack[d] = index % accumulated_products[d] / accumulated_products[d-1];
}

__host__ __device__
auto print(auto const& range) -> void{
	for(auto const c : range){
		printf("%d", c);	
	}
}

template<
	ScalarType T,
	std::size_t Nm,
	RealFunction Function
>
__global__
auto riemann_integral_kernel(
		Function const& function, 
		const range<T>* ranges, 
		const std::size_t* accumulated_products_global, 
		const T* deltas, 
		T* result_ptr
) -> void{
	auto const id=threadIdx.x + blockIdx.x*blockDim.x;

	__shared__
	math_vec<std::size_t, Nm> accumulated_products;

	if(threadIdx.x == 0){
		copy(accumulated_products_global, accumulated_products.begin(), Nm);
	}

	__syncthreads();

	if(id<accumulated_products.back()){
		math_vec<std::size_t, Nm> index_pack;
		get_index_pack(accumulated_products, id, index_pack);

		math_vec<T, Nm> point;

		for(auto i=0u;i<Nm;i++)
			point[i] = index_pack[i] * deltas[i] + ranges[i].low;

		auto res=function(point);

		result_ptr[id] = res;
	}
}

template<
	ScalarType T,
	std::size_t Nm,
	RealFunction Function
>
__global__
auto gradient_kernel(
		Function const& function, 
		const T* points,
		const T* deltas, 
		T* result_ptr
) -> void { //TODO - the question is if it is worth at all.
	auto const id=threadIdx.x + blockIdx.x*blockDim.x;

	if (id < Nm){
		
	}
}


template<
	ScalarType T,
	std::size_t kBlockSize = 64,
	RealFunction Function,
	std::size_t Nm
>
auto riemann_integral(
		Function const& function, 
		math_vec<range<T>, Nm>& ranges,
		math_vec<T, Nm> const& deltas
) -> T {

	using SizesArray=math_vec<std::size_t, Nm>;

	SizesArray dims;
	bool sign;

	::jr::calc::detailed::prepare_dims(ranges, deltas, dims, sign);

	SizesArray accumulated_products;
	std::partial_sum(dims.begin(), dims.end(), accumulated_products.begin(), std::multiplies<std::size_t>());

	auto total_count=accumulated_products.back();

	thrust::device_vector<T> result_dev_vector(total_count);
	thrust::device_vector<std::size_t> accumulated_products_dev(accumulated_products.begin(), accumulated_products.end());
	thrust::device_vector<range<T>> ranges_dev(ranges.begin(), ranges.end());
	thrust::device_vector<T> deltas_dev(deltas.begin(), deltas.end());

	riemann_integral_kernel<T, Nm><<<total_count/kBlockSize+1, kBlockSize>>>(
		function, 
		thrust::raw_pointer_cast(ranges_dev.data()), 
		thrust::raw_pointer_cast(accumulated_products_dev.data()), 
		thrust::raw_pointer_cast(deltas_dev.data()), 
		thrust::raw_pointer_cast(result_dev_vector.data())
	);

	cudaDeviceSynchronize();

	return 
		thrust::reduce(result_dev_vector.begin(), result_dev_vector.end(), T(0), thrust::plus<T>()) *
		thrust::reduce(deltas_dev.begin(), deltas_dev.end(), T(sign ? -1 : 1), thrust::multiplies<T>());
}


template<
	ScalarType T,
	std::size_t kBlockSize = 64,
	RealFunction Function,
	std::size_t Nm
>
auto calculate_gradient(
		Function const& function, 
		math_vec<T, Nm> const& points,
		math_vec<T, Nm> const& deltas
) -> thrust::device_vector<T> {

	thrust::device_vector<T> points_dev(points.begin(), points.end());
	thrust::device_vector<T> deltas_dev(deltas.begin(), deltas.end());

	thrust::device_vector<T> gradient_dev(Nm);
	
}

}

