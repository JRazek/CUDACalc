gtest_proj = subproject('gtest')
gtest_dep = dependency('gtest',  fallback : ['gtest', 'gtest_dep'])


integral_test=executable(
    'integral_test',
    'src/integral_test.cpp',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [cuda_calc_dep, gtest_dep]
)

line_integral_test=executable(
    'line_integral_test',
    'src/line_integral_test.cpp',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [cuda_calc_dep, gtest_dep]
)

fourier_series_test=executable(
    'fourier_series_test',
    'src/fourier_series_test.cpp',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [cuda_calc_dep, gtest_dep]
)


test('n-dim rectangular integral', integral_test)

test('line integral test', line_integral_test)

test('fourier series test', fourier_series_test)
