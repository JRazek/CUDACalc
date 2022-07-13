project(
    'CUDACalc', 'c', 'cpp', 'cuda',
    version: '0.1.0',
    default_options : ['c_std=c18', 'cpp_std=c++20']
)

cpp = meson.get_compiler('cpp')

thrust_dep = dependency('thrust')

incdir = include_directories(
    'include',
    'src',
)

cpp_args=[]
cpp_link_args=[]
cuda_args=[]

exec=executable(
    'CUDACalc',
    'src/main.cpp',
    include_directories : incdir,
    cpp_args: cpp_args,
	link_args: cpp_link_args,
    cuda_args: cuda_args,
)

