#ifndef GVXR_RANDOM_H
#define GVXR_RANDOM_H

#include <utility>


float mod(float x, float y);


float fract(float x);


//float generateUniformNoise(float& s);


//float generateUniformNoise(float u, float v);


unsigned int getSeed(float s);


//unsigned int getSeed(float u, float v);


unsigned TausStep(unsigned int& z, int S1, int S2, int S3, unsigned int M);


unsigned LCGStep(unsigned int& z, unsigned int A, unsigned int C);


float HybridTaus(unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4);


float generateUniformNoise(unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4);


std::pair<float, float> generateGaussianNoise(float mu, float sigma,
    unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4);


unsigned int generatePoissonNoise(unsigned int aValue,
    unsigned int& z1, unsigned int& z2, unsigned int& z3, unsigned int& z4,
    unsigned int aCutOffValue = 50);


#include "gVirtualXRay/Random.inl"

#endif
