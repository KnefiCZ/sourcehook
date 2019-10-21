#ifndef SDK_MATH_PERFORMANCE_FUNCTIONS_H
#define SDK_MATH_PERFORMANCE_FUNCTIONS_H
#pragma once

float FastSqrt(float x);
float FastRSqrtFast(float x);
float FastRSqrt(float x);
void FastSinCos(float x, float* s, float* c);
float FastCos(float x);

inline float FastClampInfinity(float x) { return (x > FLT_MAX ? FLT_MAX : x); }

#endif