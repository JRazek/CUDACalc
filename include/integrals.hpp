#pragma once

#include "function.hpp"
#include <concepts>
#include <type_traits>


namespace cuda::calc{

	template<
		RealFunction Function,
		typename... Deltas	
	>
	auto riemann_integral(Function function, Deltas... delta)
	requires(
		Function::dim_value==sizeof...(delta)
		&&
		ArithmeticParamPack<Deltas...>
	){
		
	}
}
