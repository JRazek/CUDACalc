#pragma once

#include <concepts>
#include <type_traits>
#include <array>
#include <tuple>
#include <complex>

namespace jr::calc{

namespace params_counter{

template <typename T>
concept ClassType = std::is_class_v<T>;

template <typename T>
concept HasCallableOperator = requires {
    { &T::operator() };
};

template <typename Type>
struct FunctionArgs {
    using args = void;
};

template <typename Ret, typename... Ts>
struct FunctionArgs<Ret(Ts...)> {
    using args = std::tuple<Ts...>;
};

template <typename Ret, typename... Ts>
struct FunctionArgs<Ret(*)(Ts...)> {
    using args = std::tuple<Ts...>;
};

template <ClassType ClassType, typename Ret, typename... Ts>
struct FunctionArgs<Ret(ClassType::*)(Ts...)> {
    using args = std::tuple<Ts...>;
};

template <ClassType ClassType, typename Ret, typename... Ts>
struct FunctionArgs<auto (ClassType::*)(Ts...) const -> Ret> {
    using args = std::tuple<Ts...>;
};

template <typename Functor>
requires HasCallableOperator<Functor>
struct FunctionArgs<Functor> {
    using args = typename FunctionArgs<decltype(&Functor::operator())>::args;
};

template <typename T>
static constexpr auto FunctionArgsCount = std::tuple_size_v<typename FunctionArgs<T>::args>;

template <typename T>
static constexpr auto ArraySizeFromCallable = std::tuple_size_v<
	std::remove_cvref_t<
			std::tuple_element_t<0, typename FunctionArgs<T>::args>
	>
>;

template <typename T>
using FirstFunctionParam = 
	std::remove_cvref_t<
			std::tuple_element_t<0, typename FunctionArgs<T>::args>
	>;

}

template<typename T>
struct is_complex_t : public std::false_type {};

template<typename T>
struct is_complex_t<std::complex<T>> : public std::true_type {};

template<typename T>
static constexpr auto is_complex_v = is_complex_t<T>::value;

template <typename... T>
concept RealTypeParamPack = 
std::conjunction_v<std::is_arithmetic<T>...>; //arithmetic is equivalent to real

template <typename T>
concept ScalarType = 
std::is_arithmetic_v<T>
or
is_complex_v<T>;

template <typename T>
concept RealType = 
std::is_arithmetic_v<T>;

template <typename T>
concept ComplexType = 
is_complex_v<T>;

template <typename T>
concept MathVector = 
std::is_array_v<T>;

template <typename T>
concept RealFunction = true;

template <typename T>
concept VectorField = 
MathVector<params_counter::FirstFunctionParam<T>>
and
MathVector<std::invoke_result_t<T, params_counter::FirstFunctionParam<T>>>;

template <typename T>
concept ScalarField = 
MathVector<params_counter::FirstFunctionParam<T>>
and
ScalarType<std::invoke_result_t<T, params_counter::FirstFunctionParam<T>>>;

template<typename... Ts>
concept AllSame = 
sizeof...(Ts) < 2 ||
std::conjunction_v<std::is_same<std::tuple_element_t<0, std::tuple<Ts...>>, Ts>...>;

template<typename Function, typename... Args>
concept RealFunctionParams = 
RealFunction<Function>
&&
params_counter::FunctionArgsCount<Function> == sizeof...(Args)
&&
RealTypeParamPack<Args...>
&&
AllSame<Args...>;

}
