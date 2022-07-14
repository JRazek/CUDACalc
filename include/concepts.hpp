#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>

namespace cuda::calc{

template <typename T>
concept RealFunction = true;

template <typename... T>
concept ArithmeticParamPack = 
std::conjunction_v<std::is_arithmetic<T>...>;

template <typename T>
concept Arithmetic = 
std::is_arithmetic_v<T>;

template<typename... Ts>
concept AllSame = 
sizeof...(Ts) < 2 ||
std::conjunction_v<std::is_same<std::tuple_element_t<0, std::tuple<Ts...>>, Ts>...>;

template<typename Function, typename... Args>
concept RealFunctionParams = 
RealFunction<Function>
&&
Function::dim_value==sizeof...(Args)
&&
ArithmeticParamPack<Args...>
&&
AllSame<Args...>;

}
