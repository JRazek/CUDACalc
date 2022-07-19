example_inc_dir = include_directories(
    'include',
    'src',
)

#custom_target('CUDACalc_example_app', 
#command: ['clang++', '-xcuda', './main.cpp', '-stdlib=libc++', '-std=c++20', '-L/usr/local/cuda/lib64', '-lcudart_static'])



#    '-stdlib=libc++', 
#    '-L/usr/local/cuda/lib64',
#    '-lcudart_static',




exec=executable(
    'CUDACalc_example_app',
    'src/main.cpp',
    include_directories : example_inc_dir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [cuda_calc]
)


