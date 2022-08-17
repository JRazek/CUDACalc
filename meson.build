project(
    'CUDACalc', 'c', 'cpp', 
    version: '0.1.0',
    default_options : ['c_std=c18', 'cpp_std=c++20']
)

cpp = meson.get_compiler('cpp')
cmake = import('cmake')


incdir = include_directories(
    'include',
)


cpp_build_args=[
  '-stdlib=libc++', 
  '-xcuda',
]
cpp_link_args=[
  '-stdlib=libc++',
]


if get_option('build')=='release'
    cpp_build_args += ['-O3', '-Wall', '-DNDEBUG', '-g0']
else
	cpp_build_args += ['-g3', '-Wall', '-O0']
endif


thrust_dep = dependency('thrust', required: true)


cuda_lib=cpp.find_library('cudart_static', dirs: '/usr/local/cuda/lib64', required: true)

cuda_calc_library = static_library(
    'cuda_calc',
    'include/concepts.hpp',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [thrust_dep, cuda_lib]
)


cuda_calc_dep = declare_dependency(include_directories: incdir, link_with : cuda_calc_library)


subdir('example')
subdir('tests')
