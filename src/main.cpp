#include <iostream>
#include "integrals.hpp"
#include "utils.hpp"
#include <array>
#include <memory>
#include <cmath>
#include <integrals.cuh>


auto main() -> int { 

//	constexpr auto function1=[](std::array<double, 2> const& x) -> double { 
//		return std::sin(x[0]) + std::cos(x[1]);
//	};
//	auto derivative=[](double x) -> double { return 2; };
//	auto integral=[](double x) -> double { return x*x; };
	auto zero_pi_range=std::pair(0., std::numbers::pi_v<double>);
	std::array ranges{zero_pi_range, zero_pi_range};

	std::array deltas{.001, .001};

	auto res=jr::calc::riemann_integral<jr::calc::CalculationMode::cuda>(jr::calc::cuda::cuda_function_wrapper{}, ranges, deltas);


	std::cout<<res<<'\n';

	

//

	
//	std::array<std::size_t, 3> tmp{4, 4, 2};
//	int counter=0;
//	jr::calc::nested_for_loop(tmp, [&counter](auto const& arr){
//		counter++;
//
//		std::cout<<"("<<arr[0]<<", "<<arr[1]<<", "<<arr[2]<<")\n";
//	});
//
//	std::cout<<"counter="<<counter<<'\n';
}
