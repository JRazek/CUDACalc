#pragma once

#include <concepts>
#include <type_traits>

namespace cuda::calc{
	
template <typename T>
concept RealFunction = true;

template <typename... T>
concept ArithmeticParamPack = 
std::conjunction_v<std::is_arithmetic<T>...>;

struct R_RFunction{
	static constexpr auto dim_value=1u;
};

}
