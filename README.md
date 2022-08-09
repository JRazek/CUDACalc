# CUDACalc

Early stage of library for real/complex numeric analysis.

## Development stage
- [x] Riemann integral [cpu]
- [x] Riemann integral [cuda]
- [ ] Monte Carlo integral [cpu]
- [ ] Monte Carlo integral [cuda]
- [X] Gradient [cpu]
- [ ] Gradient [cuda]
- [X] Line integral(scalar field) [cpu]
- [ ] Line integral(scalar field) [cuda] - in progess...
- [ ] Fourier Series [cpu] - in progess...
- [ ] Fourier Series [cuda]
- [ ] Fourier Transform (FFT) [cpu]
- [ ] Fourier Transform (FFT) [cuda]

## Compilation
CUDA version 11.5 is required.

In order to setup meson with clang simply run
```bash
meson setup --cross-file ./cross/native.ini build
```

Then compile by running 
```bash
meson compile -C build
```

Library is header only. 
For the sake of C++20 support we are limited to clang compiler only, which happens to be able to compile CUDA code.

[Clang CUDA support](https://llvm.org/docs/CompileCudaWithLLVM.html)


## Contribution
Everyone is free to contribute.

Each new feature should be implemented along with variety of different tests. 
