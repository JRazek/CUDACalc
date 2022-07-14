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


cpp_args=[]
cpp_link_args=[]
cuda_args=['-std=c++17']

thrust_dep = dependency('thrust')

exec=executable(
    'CUDACalc',
    'src/main.cpp',
    'src/integrals.cu',
    include_directories : incdir,
    cpp_args: cpp_args,
	link_args: cpp_link_args,
    cuda_args: cuda_args,
    dependencies: [thrust_dep]
)

