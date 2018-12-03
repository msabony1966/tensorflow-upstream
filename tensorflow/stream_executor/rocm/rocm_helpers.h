/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// Common helper functions used for dealing with ROCM API datatypes.
//
// These are typically placed here for use by multiple source components (for
// example, BLAS and executor components).

#ifndef TENSORFLOW_STREAM_EXECUTOR_ROCM_ROCM_HELPERS_H_
#define TENSORFLOW_STREAM_EXECUTOR_ROCM_ROCM_HELPERS_H_

#include <stddef.h>
#include <complex>

namespace stream_executor {

template <typename ElemT>
class DeviceMemory;

namespace gpu {

// Converts a const DeviceMemory reference to its underlying typed pointer in
// ROCM
// device memory.
template <typename T>
const T *GpuMemory(const DeviceMemory<T> &mem) {
  return static_cast<const T *>(mem.opaque());
}

// Converts a (non-const) DeviceMemory pointer reference to its underlying typed
// pointer in ROCM device memory.
template <typename T>
T *GpuMemoryMutable(DeviceMemory<T> *mem) {
  return static_cast<T *>(mem->opaque());
}

static_assert(sizeof(std::complex<float>) == sizeof(GPUComplexType),
              "std::complex<float> and GPUComplexType should have the same size");
static_assert(offsetof(GPUComplexType, x) == 0,
              "The real part of GPUComplexType should appear first.");
static_assert(sizeof(std::complex<double>) == sizeof(GPUDoubleComplexType),
              "std::complex<double> and GPUDoubleComplexType should have the same "
              "size");
static_assert(offsetof(GPUDoubleComplexType, x) == 0,
              "The real part of GPUDoubleComplexType should appear first.");

// Type traits to get ROCM complex types from std::complex<>.

template <typename T>
struct GpuComplexT {
  typedef T type;
};

template <>
struct GpuComplexT<std::complex<float>> {
  typedef GPUComplexType type;
};

template <>
struct GpuComplexT<std::complex<double>> {
  typedef GPUDoubleComplexType type;
};

// Converts pointers of std::complex<> to pointers of
// GPUComplexType/GPUDoubleComplexType. No type conversion for non-complex types.

template <typename T>
inline const typename GpuComplexT<T>::type *GpuComplex(const T *p) {
  return reinterpret_cast<const typename GpuComplexT<T>::type *>(p);
}

template <typename T>
inline typename GpuComplexT<T>::type *GpuComplex(T *p) {
  return reinterpret_cast<typename GpuComplexT<T>::type *>(p);
}

// Converts values of std::complex<float/double> to values of
// GPUComplexType/GPUDoubleComplexType.
inline GPUComplexType GpuComplexValue(std::complex<float> val) {
  return {val.real(), val.imag()};
}

inline GPUDoubleComplexType GpuComplexValue(std::complex<double> val) {
  return {val.real(), val.imag()};
}

}  // namespace gpu
}  // namespace stream_executor

#endif  // TENSORFLOW_STREAM_EXECUTOR_ROCM_ROCM_HELPERS_H_
