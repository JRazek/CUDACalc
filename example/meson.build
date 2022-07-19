example_inc_dir = include_directories(
    'include',
    'src',
)

exec=executable(
    'CUDACalc_example_app',
    'src/main.cpp',
    include_directories : example_inc_dir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [cuda_calc]
)


