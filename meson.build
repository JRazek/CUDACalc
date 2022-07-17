project(
    'CUDACalc', 'c', 'cpp', 'cuda',
    version: '0.1.0',
    default_options : ['c_std=c18', 'cpp_std=c++20']
)

cpp = meson.get_compiler('cpp')
cuda = meson.get_compiler('cuda')


incdir = include_directories(
    'include',
    'src',
)


cpp_build_args=[]
cpp_link_args=[]
cuda_args=['-std=c++17', '--extended-lambda']

#if get_option('build')=='release'
#    cpp_build_args += ['-O3', '-Wall', '-DNDEBUG', '-g0', '-s']
#else
#	cpp_build_args += ['-g3','-fsanitize=undefined,float-cast-overflow,float-divide-by-zero,bounds-strict,address,', '-Wall']
#	cpp_link_args += cpp_build_args 
#endif

thrust_dep = dependency('thrust')
cuda_dep = dependency('cuda')

exec=executable(
    'CUDACalc',
    'src/main.cpp',
    'src/integrals.cu',
    include_directories : incdir,
    cpp_args: cpp_build_args,
	link_args: cpp_link_args,
    cuda_args: cuda_args,
    dependencies: [thrust_dep, cuda_dep]
)

