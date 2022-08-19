#pragma once

#include "concepts.hpp"
#include <complex>
#include <tuple>

#include "utils.hpp"
#include "integrals.hpp"

#include <iostream>

namespace jr::calc{
	
template<
	std::size_t N,
	ScalarType T,
	RealType Y
>
class FourierSeries{
public:

	auto operator()(math_vec<Y, 1> const& t) const noexcept{
		auto res = T();
		for(auto n=0u;n<N;n++){
			res += coefficients_[n] * std::exp(2 * std::numbers::pi * compl_i * transform_index(n) * t[0] / period_.size());
		}

		return res;
	}

	template<
		CalculationMode mode = CalculationMode::cpu,
		ScalarField Function 
	>
	requires 
	(mode == CalculationMode::cpu or true) //TODO - temporary
	and
	(params_counter::ArraySizeFromCallable<Function> == 1) //only one param functions allowed.
	static auto calculate_fourier_series(
			Function const& function, 
			range<Y> const& period,
			Y delta
	) -> FourierSeries {
		
		auto inv_period = 1./(period.high - period.low);

		math_vec<range<Y>, 1> helper_range = {period};
		math_vec<Y, 1> helper_deltas = {delta};

		FourierSeries series;
		series.period_ = period;

		for(auto n = 0u;n<N;n++){
			auto helper_function = [&function, n = transform_index(n), &period](math_vec<Y, 1> const& t){ 
				return function(t) * std::exp(2 * std::numbers::pi * -n * compl_i * t[0] / period.size());
			}; //f(x) * exp(-int/T) 
			   
			series.coefficients_[n] = inv_period * riemann_integral<mode>(helper_function, helper_range, helper_deltas);
		}

		return series;
	}

	static auto transform_index(std::size_t n) -> std::int64_t{
		constexpr static auto shift = (N-1)/2;
		return n - shift;
	}

private:
	constexpr static auto inline compl_i = std::complex{0., 1.};

	range<Y> period_;
	math_vec<T, N> coefficients_; //..., -2, -1, 0, 1, 2, ...
};



}
