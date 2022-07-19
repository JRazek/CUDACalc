project(
    'CUDACalc', 'c', 'cpp', 
    version: '0.1.0',
    default_options : ['c_std=c18', 'cpp_std=c++20']
)

cpp = meson.get_compiler('cpp')

incdir = include_directories(
    'include',
)


cpp_build_args=[
  '-stdlib=libc++', 
  '--cuda-gpu-arch=sm_70', 
  '-xcuda',
]
cpp_link_args=[
  '-stdlib=libc++',
  '--cuda-gpu-arch=sm_70', 
]


cpp_build_args += ['-O3', '-Wall', '-DNDEBUG', '-g0']
#if get_option('build')=='release'
#    cpp_build_args += ['-O3', '-Wall', '-DNDEBUG', '-g0', '-s']
#else
#	cpp_build_args += ['-g3','-fsanitize=undefined,float-cast-overflow,float-divide-by-zero,bounds-strict,address,', '-Wall']
#	cpp_link_args += cpp_build_args 
#endif


thrust_dep = dependency('thrust')

cuda_lib=cpp.find_library('cudart_static', dirs: '/usr/local/cuda/lib64', required: true)

cuda_calc_library = static_library(
    'cuda_calc',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    dependencies: [thrust_dep, cuda_lib]
)


cuda_calc = declare_dependency(include_directories: incdir, link_with : cuda_calc_library)


subdir('example')

