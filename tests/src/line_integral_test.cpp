#include "gtest/gtest.h"

#include "integrals.hpp"

using namespace jr::calc;

TEST(LineIntegral, StraightLine){
	auto line = [](math_vec<double, 1> const& x){
		return x;
	};

	auto integral = [](range<double> const& range){
		return 4*(range.high - range.low);
	};

	auto scalar_field = [](math_vec<double, 1> const& x){
		return 4.;
	};
	
	double delta = 0.01;
	
	range<double> range{-1., 1.};

	auto numerical_res = calculate_line_integral(scalar_field, range, line, delta);

	EXPECT_NEAR(integral(range), numerical_res, 0.1);
}

auto main() -> int{
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
