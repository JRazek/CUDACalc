#pragma once

#include <concepts>
#include <type_traits>
#include <tuple>

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

}

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
params_counter::FunctionArgsCount<Function> == sizeof...(Args)
&&
ArithmeticParamPack<Args...>
&&
AllSame<Args...>;

}
