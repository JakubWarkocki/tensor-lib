#include "vectorised_ops.h"

#define USE_AVX2

#if defined(USE_AVX2)

#include <immintrin.h>

#elif defined(USE_AVX512)

#include <immintrin.h>

#elif defined(USE_SSE2)

#include <emmintrin.h>

#endif

void vector_dot_product(const float* restrict v1, const float* restrict v2, float* restrict dst, const int vlen) {
  
  float sum = 0.0f;

#if defined(USE_SSE2)

  int i = 0;
  __m128 vsum = _mm_setzero_ps();

  for (; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 prod = _mm_mul_ps(a, b);
    vsum = _mm_add_ps(vsum, prod);
  }
  
  float temp[4];
  _mm_storeu_ps(temp, vsum);
  for (int j = 0; j < 4; ++j) {
    sum += temp[j];
  }

  for (; i < vlen; ++i) {
    sum += v1[i] * v2[i];
  }

#elif defined(USE_AVX2)

  int i = 0;
  __m256 vsum = _mm256_setzero_ps();

  for (; i <= vlen - 8; i += 8) {
    __m256 a = _mm256_loadu_ps(&v1[i]);
    __m256 b = _mm256_loadu_ps(&v2[i]);
    __m256 prod = _mm256_mul_ps(a, b);
    vsum = _mm256_add_ps(vsum, prod);
  }

  __m128 vsum_low = _mm256_castps256_ps128(vsum);
  __m128 vsum_high = _mm256_extractf128_ps(vsum, 1);
  vsum_low = _mm_add_ps(vsum_low, vsum_high);

  for(; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 prod = _mm_mul_ps(a, b);
    vsum_low = _mm_add_ps(vsum_low, prod);
  }

  float temp[4];
  _mm_storeu_ps(temp, vsum_low);
  for (int j = 0; j < 4; ++j) {
    sum += temp[j];
  }

  for (; i < vlen; ++i) {
    sum += v1[i] * v2[i];
  }

#elif defined(USE_AVX512)
  
  int i = 0;
  __m512 vsum = _mm512_setzero_ps();
  for (; i <= vlen - 16; i += 16) {
    __m512 a = _mm512_loadu_ps(&v1[i]);
    __m512 b = _mm512_loadu_ps(&v2[i]);
    __m512 prod = _mm512_mul_ps(a, b);
    vsum = _mm512_add_ps(vsum, prod);
  }

  __m256 vsum_low = _mm512_castps512_ps256(vsum);
  __m256 vsum_high = _mm512_extractf32x8_ps(vsum, 1);
  vsum_low = _mm256_add_ps(vsum_low, vsum_high);

  for (; i <= vlen - 8; i += 8) {
    __m256 a = _mm256_loadu_ps(&v1[i]);
    __m256 b = _mm256_loadu_ps(&v2[i]);
    __m256 prod = _mm256_mul_ps(a, b);
    vsum_low = _mm256_add_ps(vsum_low, prod);
  }
  
  __m128 vsum_low_128 = _mm256_castps256_ps128(vsum_low);
  __m128 vsum_high_128 = _mm256_extractf128_ps(vsum_low, 1);
  vsum_low_128 = _mm_add_ps(vsum_low_128, vsum_high_128);

  for (; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 prod = _mm_mul_ps(a, b);
    vsum_low_128 = _mm_add_ps(vsum_low_128, prod);
  }

  float temp[4];
  _mm_storeu_ps(temp, vsum_low_128);
  for (int j = 0; j < 4; ++j) {
    sum += temp[j];
  }

  for (; i < vlen; ++i) {
    sum += v1[i] * v2[i];
  }

#else

  for (int i = 0; i < vlen; ++i) {
    sum += v1[i] * v2[i];
  }

#endif

  *dst = sum;

}


void vector_element_wise_mul(const float * restrict v1, const float * restrict v2, float* restrict dst, const int vlen) {
#if defined (USE_SSE2)

  int i = 0;

  for(; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 prod = _mm_mul_ps(a, b);
    _mm_storeu_ps(&dst[i], prod);
  }

  for(; i < vlen; i++) {
    dst[i] = v1[i] * v2[i];
  }

#elif defined(USE_AVX2)

  int i = 0;

  for(; i <= vlen - 8; i += 8) {
    __m256 a = _mm256_loadu_ps(&v1[i]);
    __m256 b = _mm256_loadu_ps(&v2[i]);
    __m256 prod = _mm256_mul_ps(a, b);
    _mm256_storeu_ps(&dst[i], prod);
  }

  for(; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 prod = _mm_mul_ps(a, b);
    _mm_storeu_ps(&dst[i], prod);
  }
  
  for(; i < vlen; i++) {
    dst[i] = v1[i] * v2[i];
  }


#else

  for(int i=0; i<vlen; i++) {
    dst[i] = v1[i] * v2[i];
  }

#endif
}

void vector_add(const float * restrict v1, const float * restrict v2, float* restrict dst, const int vlen) {

#if defined (USE_SSE2)

  int i = 0;
  for(; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 sum = _mm_add_ps(a, b);
    _mm_storeu_ps(&dst[i], sum);
  }
  for(; i < vlen; i++) {
    dst[i] = v1[i] + v2[i];
  }

#elif defined(USE_AVX2) 

  int i = 0;
  for(; i <= vlen - 8; i += 8) {
    __m256 a = _mm256_loadu_ps(&v1[i]);
    __m256 b = _mm256_loadu_ps(&v2[i]);
    __m256 sum = _mm256_add_ps(a, b);
    _mm256_storeu_ps(&dst[i], sum);
  }

  for(; i <= vlen - 4; i += 4) {
    __m128 a = _mm_loadu_ps(&v1[i]);
    __m128 b = _mm_loadu_ps(&v2[i]);
    __m128 sum = _mm_add_ps(a, b);
    _mm_storeu_ps(&dst[i], sum);
  }

  for(; i < vlen; i++) {
    dst[i] = v1[i] + v2[i];
  }

#else

  for(int i=0; i<vlen; i++) {
    dst[i] = v1[i] + v2[i];
  }

#endif
}

void vector_add_mul(const float * restrict v1, const float * restrict v2, const float sca, float* restrict dst, const int vlen) {
  for(int i=0; i<vlen; i++) {
    dst[i] = v1[i] + (sca * v2[i]);
  }
}

void vector_scalar_mul(const float * restrict v1, const float sca, float* restrict dst, const int vlen) {
  for(int i=0; i<vlen; i++) {
    dst[i] = v1[i] * sca;
  }
}

void vector_scalar_add(const float * restrict v1, const float sca, float* restrict dst, const int vlen) {
  for(int i=0; i<vlen; i++) {
    dst[i] = v1[i] + sca;
  }
}

void vector_scalar_set(float * restrict dst, const float sca, const int vlen){
  for(int i=0; i<vlen; i++) {
    dst[i] = sca;
  }
}

void vector_copy(const float * restrict src, float* restrict dst, const int vlen, const int src_stride) {
  for(int i=0; i<vlen; i++) {
    dst[i] = src[i];
  }
}

