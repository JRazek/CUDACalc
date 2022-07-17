#pragma once

#include <array>


#ifdef __NVCC__

#include <cuda/std/array>
#include <cuda/std/cstddef>
#include <thrust/device_vector.h>

#endif //__NVCC__

namespace jr{
namespace calc{
namespace cuda{


struct cuda_function_wrapper{
#ifdef __NVCC__


__device__
auto operator()() const -> void{

}


#endif //__NVCC__
};

#ifdef  __NVCC__


#endif //__NVCC__



}
}
}
