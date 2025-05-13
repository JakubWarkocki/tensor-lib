#ifndef VECTORISED_OPS_H
#define VECTORISED_OPS_H

void vector_dot_product(const float* v1, const float* v2, float* dst, const int vlen);

void vector_element_wise_mul(const float * v1, const float * v2, float* dst, const int vlen);

void vector_add(const float * v1, const float * v2, float* dst, const int vlen);

void vector_add_mul(const float * v1, const float * v2, const float sca, float* dst, const int vlen);

void vector_scalar_mul(const float * v1, const float sca, float* dst, const int vlen);

void vector_scalar_add(const float * v1, const float sca, float* dst, const int vlen);

void vector_scalar_set(float * dst, const float sca, const int vlen);

void vector_copy(const float * src, float* dst, const int vlen);

#endif // VECTORISED_OPS_H
