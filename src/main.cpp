#include <iostream>
#include "integrals.hpp"
#include "utils.hpp"
#include <array>
#include <memory>


auto main() -> int { 

	constexpr auto function1=[](std::array<double, 2> const& x) -> double { 
		return 1;
	};
//	auto derivative=[](double x) -> double { return 2; };
//	auto integral=[](double x) -> double { return x*x; };

	std::array ranges{std::pair(0., 1.), std::pair(0., 1.)};
	std::array deltas{.1, .1};

//	auto res=jr::calc::riemann_integral<decltype(function1), double, jr::calc::CalculationMode::cpu>(function1, ranges, deltas);


//	std::cout<<res<<'\n';
//

	
	std::array<std::size_t, 3> tmp{4, 4, 2};
	int counter=0;
	jr::calc::nested_for_loop(tmp, [&counter](auto const& arr){
		counter++;

		std::cout<<"("<<arr[0]<<", "<<arr[1]<<", "<<arr[2]<<")\n";
	});

	std::cout<<"counter="<<counter<<'\n';
}
