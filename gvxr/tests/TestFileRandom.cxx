#include <omp.h>
//#include <bits/stdc++.h>

#include <cmath>

#include <algorithm>
#include <chrono>

#include "gtest/gtest.h"

#ifndef GVXR_RANDOM_H
#include "gVirtualXRay/Random.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

typedef std::chrono::time_point<std::chrono::high_resolution_clock>  TimePoint;


//******************************************************************************
//  Const
//******************************************************************************
namespace Testing {
    const size_t g_image_width = 2048;
    const size_t g_image_height = 2048;
    const size_t g_repeat_iteration = 1;
}


//------------------------------------
TEST(TestFileRandom, TestUniformNoise)
//------------------------------------
{

    float mu = 20.0;
    float sigma = 5.0;

    gVirtualXRay::Image<float> uniform_noise_image(Testing::g_image_width, Testing::g_image_height);
    int max_size = uniform_noise_image.getWidth() * uniform_noise_image.getHeight() * uniform_noise_image.getDepth();
    float* p_temp = uniform_noise_image.getRawData();

    TimePoint start_uniform_noise_cpu;
    TimePoint stop_uniform_noise_cpu;

    TimePoint start_average_cpu;
    TimePoint stop_average_cpu;

    TimePoint start_variance_cpu;
    TimePoint stop_variance_cpu;

    TimePoint start_min_max_cpu;
    TimePoint stop_min_max_cpu;
    

    TimePoint start_uniform_noise_openmp_cpu;
    TimePoint stop_uniform_noise_openmp_cpu;

    TimePoint start_average_openmp_cpu;
    TimePoint stop_average_openmp_cpu;

    TimePoint start_variance_openmp_cpu;
    TimePoint stop_variance_openmp_cpu;

    TimePoint start_min_max_openmp_cpu;
    TimePoint stop_min_max_openmp_cpu;
    
    
    TimePoint start_uniform_noise_openmp_gpu;
    TimePoint stop_uniform_noise_openmp_gpu;

    TimePoint start_average_openmp_gpu;
    TimePoint stop_average_openmp_gpu;

    TimePoint start_variance_openmp_gpu;
    TimePoint stop_variance_openmp_gpu;

    TimePoint start_min_max_openmp_gpu;
    TimePoint stop_min_max_openmp_gpu;
    
    // CPU without OpenMP
    {
        start_uniform_noise_cpu = high_resolution_clock::now();

        for (int i = 0; i < max_size; ++i)
        {
            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            p_temp[i] = mu + sigma * (generateUniformNoise(z1, z2, z3, z4) - 0.5);
        }
        stop_uniform_noise_cpu = high_resolution_clock::now();

        start_average_cpu = high_resolution_clock::now();
        float average = 0.0F;
        for (int i = 0; i < max_size; ++i)
        {
            average += p_temp[i];
        }
        average /= static_cast<float>(max_size);
        stop_average_cpu = high_resolution_clock::now();

        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_cpu = high_resolution_clock::now();
        for (int i = 1; i < max_size; ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_cpu = high_resolution_clock::now();

        float error_mu = 100.0F * (average - mu) / mu;
        float ref_min_value = mu - sigma / 2.0F;
        float ref_max_value = mu + sigma / 2.0F;
        float error_min = 100.0F * (min_value - ref_min_value) / ref_min_value;
        float error_max = 100.0F * (max_value - ref_max_value) / ref_max_value;

        std::cout << "CPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << "\terror:\t" << error_min << "%" << std::endl;
        std::cout << "\tmax_value:\t" << max_value << "\terror:\t" << error_max << "%" << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;

        uniform_noise_image.saveTIFF("uniform_noise_cpu.tif");

        if (Testing::g_repeat_iteration > 1)
        {
            ASSERT_NEAR(error_mu, 0, 5.0);
            ASSERT_NEAR(error_min, 0, 0.5);
            ASSERT_NEAR(error_max, 0, 0.5);
        }
    }
    
    // CPU with OpenMP
    {
        start_uniform_noise_openmp_cpu = high_resolution_clock::now();
        int is_device = 1;

        #pragma omp parallel
        {
            // Get the thread ID
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            #pragma omp parallel for
            for (int i = 0; i < max_size; ++i)
            {
                unsigned int z1 = getSeed(i * 4 + 0);
                unsigned int z2 = getSeed(i * 4 + 1);
                unsigned int z3 = getSeed(i * 4 + 2);
                unsigned int z4 = getSeed(i * 4 + 3);

                p_temp[i] = mu + sigma * (generateUniformNoise(z1, z2, z3, z4) - 0.5F);
            }
        }
        stop_uniform_noise_openmp_cpu = high_resolution_clock::now();

        start_average_openmp_cpu = high_resolution_clock::now();
        float average = 0.0F;

        #pragma omp parallel for reduction(+: average)
        for (int i = 0; i < max_size; ++i)
        {
            average += p_temp[i];
        }
        average /= max_size;
        stop_average_openmp_cpu = high_resolution_clock::now();

        start_variance_openmp_cpu = high_resolution_clock::now();
        float variance = 0.0;

        #pragma omp parallel for reduction(+: variance)
        for (int i = 0; i < max_size; ++i)
        {
            variance += (p_temp[i] - average) * (p_temp[i] - average);
        }
        variance /= static_cast<float>(max_size);
        stop_variance_openmp_cpu = high_resolution_clock::now();

        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_openmp_cpu = high_resolution_clock::now();

#ifdef NDEBUG
#ifndef WIN32
        #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
#endif
#endif
        for (int i = 1; i < max_size; ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_openmp_cpu = high_resolution_clock::now();

        float error_mu = 100.0 * (average - mu) / mu;
        float ref_min_value = mu - sigma / 2.0;
        float ref_max_value = mu + sigma / 2.0;
        float error_min = 100.0 * (min_value - ref_min_value) / ref_min_value;
        float error_max = 100.0 * (max_value - ref_max_value) / ref_max_value;

        std::cout << "Running on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;
        std::cout << "OpenMP\tCPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << "\terror:\t" << error_min << "%" << std::endl;
        std::cout << "\tmax_value:\t" << max_value << "\terror:\t" << error_max << "%" << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;

        uniform_noise_image.saveTIFF("uniform_noise_openmp_cpu.tif");

        if (Testing::g_repeat_iteration > 1)
        {
            ASSERT_NEAR(error_mu, 0, 5.0);
            ASSERT_NEAR(error_min, 0, 0.5);
            ASSERT_NEAR(error_max, 0, 0.5);
        }
    }

    // GPU with OpenMP
    {
        start_uniform_noise_openmp_gpu = high_resolution_clock::now();
        int is_device = 1;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(from:p_temp[0:max_size], is_device)
        #else
        #pragma omp parallel for
        #endif
        for (int i = 0; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            p_temp[i] = mu + sigma * (generateUniformNoise(z1, z2, z3, z4) - 0.5);
        }
        stop_uniform_noise_openmp_gpu = high_resolution_clock::now();

        is_device = -1;
        start_average_openmp_gpu = high_resolution_clock::now();
        float average = 0.0;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams map(to:p_temp[:max_size]) map(tofrom:average) shared(average)
        #endif
        {
            #ifdef HAS_OPENMP_GPU
            #pragma omp distribute parallel for reduction(+: average)
            #else
            #pragma omp parallel for reduction(+: average)
            #endif
            for (int i = 0; i < max_size; ++i)
            {
                // is_device = omp_is_initial_device();

                average += p_temp[i];
            }
        }
        average /= max_size;
        stop_average_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (average) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        is_device = -1;
        start_variance_openmp_gpu = high_resolution_clock::now();
        float variance = 0.0;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(to:p_temp[:max_size]) map(tofrom:variance)  map(tofrom:is_device) reduction(+: variance)
        #else
        #pragma omp parallel for reduction(+: variance)
        #endif
        for (int i = 0; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            variance += (p_temp[i] - average) * (p_temp[i] - average);
        }
        variance /= static_cast<float>(max_size);
        stop_variance_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (variance) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        float min_value = p_temp[0];
        float max_value = p_temp[0];
        is_device = -1;
        start_min_max_openmp_gpu = high_resolution_clock::now();

        #ifdef NDEBUG
        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(to:p_temp[:max_size]) map(tofrom:min_value, max_value)  map(tofrom:is_device) reduction(min: min_value) reduction(max: max_value)
        #else // HAS_OPENMP_GPU
        #ifndef WIN32
        #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
        #endif // WIN32
        #endif // HAS_OPENMP_GPU
        #endif // NDEBUG
        for (int i = 1; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }      
        stop_min_max_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (min/max) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        float error_mu = 100.0 * (average - mu) / mu;
        float ref_min_value = mu - sigma / 2.0;
        float ref_max_value = mu + sigma / 2.0;
        float error_min = 100.0 * (min_value - ref_min_value) / ref_min_value;
        float error_max = 100.0 * (max_value - ref_max_value) / ref_max_value;

        std::cout << "Running on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;
        std::cout << "OpenMP\tCPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << "\terror:\t" << error_min << "%" << std::endl;
        std::cout << "\tmax_value:\t" << max_value << "\terror:\t" << error_max << "%" << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;

        uniform_noise_image.saveTIFF("uniform_noise_openmp_gpu.tif");

        if (Testing::g_repeat_iteration > 1)
        {
            ASSERT_NEAR(error_mu, 0, 5.0);
            ASSERT_NEAR(error_min, 0, 0.5);
            ASSERT_NEAR(error_max, 0, 0.5);
        }
    }


    std::cout << "Uniform noise\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_uniform_noise_cpu - start_uniform_noise_cpu).count() << std::endl;
    std::cout << "Average\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_average_cpu - start_average_cpu).count() << std::endl;
    std::cout << "Variance\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_variance_cpu - start_variance_cpu).count() << std::endl;
    std::cout << "Min/Max\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_min_max_cpu - start_min_max_cpu).count() << std::endl << std::endl;

    std::cout << "Uniform noise\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_uniform_noise_openmp_cpu - start_uniform_noise_openmp_cpu).count() << std::endl;
    std::cout << "Average\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_average_openmp_cpu - start_average_openmp_cpu).count() << std::endl;
    std::cout << "Variance\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_variance_openmp_cpu - start_variance_openmp_cpu).count() << std::endl;
    std::cout << "Min/Max\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_min_max_openmp_cpu - start_min_max_openmp_cpu).count() << std::endl << std::endl;

    std::cout << "Uniform noise\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_uniform_noise_openmp_gpu - start_uniform_noise_openmp_gpu).count() << std::endl;
    std::cout << "Average\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_average_openmp_gpu - start_average_openmp_gpu).count() << std::endl;
    std::cout << "Variance\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_variance_openmp_gpu - start_variance_openmp_gpu).count() << std::endl;
    std::cout << "Min/Max\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_min_max_openmp_gpu - start_min_max_openmp_gpu).count() << std::endl << std::endl;
}


//-------------------------------------
TEST(TestFileRandom, TestGaussianNoise)
//-------------------------------------
{

    float mu = 20.0;
    float sigma = 5.0;

    gVirtualXRay::Image<float> gaussian_noise_image(Testing::g_image_width, Testing::g_image_height);
    int max_size = gaussian_noise_image.getWidth() * gaussian_noise_image.getHeight() * gaussian_noise_image.getDepth();
    float* p_temp = gaussian_noise_image.getRawData();

    unsigned int initial_seed = time(0);

    TimePoint start_gaussian_noise_cpu;
    TimePoint stop_gaussian_noise_cpu;

    TimePoint start_average_cpu;
    TimePoint stop_average_cpu;

    TimePoint start_variance_cpu;
    TimePoint stop_variance_cpu;

    TimePoint start_min_max_cpu;
    TimePoint stop_min_max_cpu;
    

    TimePoint start_gaussian_noise_openmp_cpu;
    TimePoint stop_gaussian_noise_openmp_cpu;

    TimePoint start_average_openmp_cpu;
    TimePoint stop_average_openmp_cpu;

    TimePoint start_variance_openmp_cpu;
    TimePoint stop_variance_openmp_cpu;

    TimePoint start_min_max_openmp_cpu;
    TimePoint stop_min_max_openmp_cpu;
    
    
    TimePoint start_gaussian_noise_openmp_gpu;
    TimePoint stop_gaussian_noise_openmp_gpu;

    TimePoint start_average_openmp_gpu;
    TimePoint stop_average_openmp_gpu;

    TimePoint start_variance_openmp_gpu;
    TimePoint stop_variance_openmp_gpu;

    TimePoint start_min_max_openmp_gpu;
    TimePoint stop_min_max_openmp_gpu;
    
    // CPU without OpenMP
    {
        start_gaussian_noise_cpu = high_resolution_clock::now();

        for (int i = 0; i < max_size / 2; ++i)
        {
            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
            p_temp[i * 2 + 0] = random_floats.first;
            p_temp[i * 2 + 1] = random_floats.second;
        }
        stop_gaussian_noise_cpu = high_resolution_clock::now();

        start_average_cpu = high_resolution_clock::now();
        float average = 0.0;
        for (int i = 0; i < max_size; ++i)
        {
            average += p_temp[i];
        }
        average /= max_size;
        stop_average_cpu = high_resolution_clock::now();

        start_variance_cpu = high_resolution_clock::now();
        float variance = 0.0;
        for (int i = 0; i < max_size; ++i)
        {
            variance += (p_temp[i] - average) * (p_temp[i] - average);
        }
        variance /= static_cast<float>(max_size);
        stop_variance_cpu = high_resolution_clock::now();

        float std_dev = std::sqrt(variance);

        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_cpu = high_resolution_clock::now();
        for (int i = 1; i < max_size; ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_cpu = high_resolution_clock::now();

        float error_mu = 100.0 * (average - mu) / mu;
        float error_sigma = 100.0 * (std_dev - sigma) / sigma;

        std::cout << "CPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << std::endl;
        std::cout << "\tmax_value:\t" << max_value << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;
        std::cout << "\tvariance:\t" << variance << std::endl;
        std::cout << "\tstd_dev:\t" << std_dev << "\terror:\t" << error_sigma << "%" << std::endl << std::endl;

        gaussian_noise_image.saveTIFF("gaussian_noise_cpu.tif");

        ASSERT_NEAR(error_mu, 0, 5.0);
        ASSERT_NEAR(error_sigma, 0, 5.0);
    }
    
    // CPU with OpenMP
    {
        start_gaussian_noise_openmp_cpu = high_resolution_clock::now();
        int is_device = 1;

        #pragma omp parallel
        {
            // Get the thread ID
            int thread_id = omp_get_thread_num();

            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            // omp_get_num_threads() * 4 + 
            unsigned int z1 = initial_seed + thread_id * 4 + 0;
            unsigned int z2 = initial_seed + thread_id * 4 + 1;
            unsigned int z3 = initial_seed + thread_id * 4 + 2;
            unsigned int z4 = initial_seed + thread_id * 4 + 3;

            #pragma omp parallel for
            for (int i = 0; i < max_size / 2; ++i)
            {
                std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
                p_temp[i * 2 + 0] = random_floats.first;
                p_temp[i * 2 + 1] = random_floats.second;
            }
        }
        stop_gaussian_noise_openmp_cpu = high_resolution_clock::now();

        start_average_openmp_cpu = high_resolution_clock::now();
        float average = 0.0;

        #pragma omp parallel for reduction(+: average)
        for (int i = 0; i < max_size; ++i)
        {
            average += p_temp[i];
        }
        average /= max_size;
        stop_average_openmp_cpu = high_resolution_clock::now();

        start_variance_openmp_cpu = high_resolution_clock::now();
        float variance = 0.0;

        #pragma omp parallel for reduction(+: variance)
        for (int i = 0; i < max_size; ++i)
        {
            variance += (p_temp[i] - average) * (p_temp[i] - average);
        }
        variance /= static_cast<float>(max_size);
        stop_variance_openmp_cpu = high_resolution_clock::now();

        float std_dev = std::sqrt(variance);


        float min_value = p_temp[0];
        float max_value = p_temp[0];

        start_min_max_openmp_cpu = high_resolution_clock::now();

#ifdef NDEBUG
#ifndef WIN32
        #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
#endif
#endif
        for (int i = 1; i < max_size; ++i)
        {
            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }        
        stop_min_max_openmp_cpu = high_resolution_clock::now();

        float error_mu = 100.0 * (average - mu) / mu;
        float error_sigma = 100.0 * (std_dev - sigma) / sigma;

        std::cout << "Running on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;
        std::cout << "OpenMP\tCPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << std::endl;
        std::cout << "\tmax_value:\t" << max_value << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;
        std::cout << "\tvariance:\t" << variance << std::endl;
        std::cout << "\tstd_dev:\t" << std_dev << "\terror:\t" << error_sigma << "%" << std::endl << std::endl;

        gaussian_noise_image.saveTIFF("gaussian_noise_openmp_cpu.tif");

        ASSERT_NEAR(error_mu, 0, 5.0);
        ASSERT_NEAR(error_sigma, 0, 5.0);
    }

    // GPU with OpenMP
    {
        start_gaussian_noise_openmp_gpu = high_resolution_clock::now();
        int is_device = 1;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(from:p_temp[0:max_size], is_device)
        #else
        #pragma omp parallel for
        #endif
        for (int i = 0; i < max_size / 2; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
            p_temp[i * 2 + 0] = random_floats.first;
            p_temp[i * 2 + 1] = random_floats.second;
        }
        stop_gaussian_noise_openmp_gpu = high_resolution_clock::now();

        is_device = -1;
        start_average_openmp_gpu = high_resolution_clock::now();
        float average = 0.0;
        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams map(to:p_temp[:max_size]) map(tofrom:average) shared(average)
        #endif
        {
            #ifdef HAS_OPENMP_GPU
            #pragma omp distribute parallel for reduction(+: average)
            #else
            #pragma omp parallel for reduction(+: average)
            #endif
            for (int i = 0; i < max_size; ++i)
            {
                average += p_temp[i];
            }
        }
        average /= max_size;
        stop_average_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (average) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        is_device = -1;
        start_variance_openmp_gpu = high_resolution_clock::now();
        float variance = 0.0;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(to:p_temp[:max_size]) map(tofrom:variance)  map(tofrom:is_device) reduction(+: variance)
        #else
        #pragma omp parallel for reduction(+: variance)
        #endif
        for (int i = 0; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            variance += (p_temp[i] - average) * (p_temp[i] - average);
        }
        variance /= static_cast<float>(max_size);
        stop_variance_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (variance) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        float std_dev = std::sqrt(variance);

        float min_value = p_temp[0];
        float max_value = p_temp[0];
        is_device = 1;
        start_min_max_openmp_gpu = high_resolution_clock::now();

#ifdef NDEBUG
#ifdef HAS_OPENMP_GPU
#pragma omp target teams distribute parallel for map(to:p_temp[:max_size]) map(tofrom:min_value, max_value)  map(tofrom:is_device) reduction(min: min_value) reduction(max: max_value)
#else // HAS_OPENMP_GPU
#ifndef WIN32
#pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
#endif // WIN32
#endif // HAS_OPENMP_GPU
#endif // NDEBUG
        for (int i = 1; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            min_value = std::min(min_value, p_temp[i]);
            max_value = std::max(max_value, p_temp[i]);
        }      
        stop_min_max_openmp_gpu = high_resolution_clock::now();
        std::cout << "Running (min/max) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;

        float error_mu = 100.0 * (average - mu) / mu;
        float error_sigma = 100.0 * (std_dev - sigma) / sigma;

        std::cout << "OpenMP\tGPU:\t" << std::endl;
        std::cout << "\tmin_value:\t" << min_value << std::endl;
        std::cout << "\tmax_value:\t" << max_value << std::endl;
        std::cout << "\taverage:\t" << average << "\terror:\t" << error_mu << "%" << std::endl;
        std::cout << "\tvariance:\t" << variance << std::endl;
        std::cout << "\tstd_dev:\t" << std_dev << "\terror:\t" << error_sigma << "%" << std::endl << std::endl;

        gaussian_noise_image.saveTIFF("gaussian_noise_openmp_gpu.tif");

        ASSERT_NEAR(error_mu, 0.0F, 5.0F);
        ASSERT_NEAR(error_sigma, 0.0F, 5.0F);
    }


    std::cout << "Gaussian noise\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_gaussian_noise_cpu - start_gaussian_noise_cpu).count() << std::endl;
    std::cout << "Average\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_average_cpu - start_average_cpu).count() << std::endl;
    std::cout << "Variance\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_variance_cpu - start_variance_cpu).count() << std::endl;
    std::cout << "Min/Max\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_min_max_cpu - start_min_max_cpu).count() << std::endl << std::endl;

    std::cout << "Gaussian noise\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_gaussian_noise_openmp_cpu - start_gaussian_noise_openmp_cpu).count() << std::endl;
    std::cout << "Average\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_average_openmp_cpu - start_average_openmp_cpu).count() << std::endl;
    std::cout << "Variance\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_variance_openmp_cpu - start_variance_openmp_cpu).count() << std::endl;
    std::cout << "Min/Max\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_min_max_openmp_cpu - start_min_max_openmp_cpu).count() << std::endl << std::endl;

    std::cout << "Gaussian noise\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_gaussian_noise_openmp_gpu - start_gaussian_noise_openmp_gpu).count() << std::endl;
    std::cout << "Average\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_average_openmp_gpu - start_average_openmp_gpu).count() << std::endl;
    std::cout << "Variance\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_variance_openmp_gpu - start_variance_openmp_gpu).count() << std::endl;
    std::cout << "Min/Max\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_min_max_openmp_gpu - start_min_max_openmp_gpu).count() << std::endl << std::endl;
}


//------------------------------------
TEST(TestFileRandom, TestPoissonNoise)
//------------------------------------
{
    gVirtualXRay::Image<float> gaussian_noise_image(Testing::g_image_width, Testing::g_image_height);
    gVirtualXRay::Image<unsigned int> poisson_noise_image(Testing::g_image_width, Testing::g_image_height);
    int max_size = static_cast<int>(poisson_noise_image.getWidth() * poisson_noise_image.getHeight() *
        poisson_noise_image.getDepth());
    float mu = 70.0F;
    float sigma = 20.0F;

    float* p_gaussian_noise = gaussian_noise_image.getRawData();
    unsigned int* p_poisson_noise = poisson_noise_image.getRawData();

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(from:p_gaussian_noise[0:max_size]) 
        #else
        #pragma omp parallel for
        #endif
        for (int i = 0; i < max_size / 2; ++i)
        {
            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            std::pair<float, float> random_floats = generateGaussianNoise(mu, sigma, z1, z2, z3, z4);
            p_gaussian_noise[i * 2 + 0] = random_floats.first;
            p_gaussian_noise[i * 2 + 1] = random_floats.second;
        }
    
    gaussian_noise_image.saveTIFF("gaussian_noise_for_poisson.tif");

    TimePoint start_poisson_noise_cpu;
    TimePoint stop_poisson_noise_cpu;

    TimePoint start_poisson_noise_openmp_cpu;
    TimePoint stop_poisson_noise_openmp_cpu;

    TimePoint start_poisson_noise_openmp_gpu;
    TimePoint stop_poisson_noise_openmp_gpu;

    // CPU without OpenMP
    {
        start_poisson_noise_cpu = high_resolution_clock::now();

        for (int i = 0; i < max_size; ++i)
        {
            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            p_poisson_noise[i] = 0;

            for (size_t j = 0; j < Testing::g_repeat_iteration; ++j)
            {
                p_poisson_noise[i] += generatePoissonNoise(static_cast<unsigned int>(i), z1, z2, z3, z4, 50);
            }

            p_poisson_noise[i] /= Testing::g_repeat_iteration;
        }
        stop_poisson_noise_cpu = high_resolution_clock::now();

        for (int i = 0; i < max_size; ++i)
        {
            unsigned int random_value = p_poisson_noise[i];
            float absolute_error = std::abs(static_cast<float>(random_value) - static_cast<float>(i));
            float relative_error = 0.0F;

            if (i > 0)
            {
                relative_error = 100.0F * absolute_error / static_cast<float>(i);
            }

	//    std::cout << i << "\t" << random_value << "\t" << absolute_error << "\t" << relative_error << std::endl;
	    
	        if (Testing::g_repeat_iteration > 5000)
	        {
	            ASSERT_EQ(absolute_error < 5.0, true);
	            ASSERT_EQ(relative_error < 1.5, true);
	        }

	    ++i;
        }
    }

    // CPU with OpenMP
    {
        start_poisson_noise_openmp_cpu = high_resolution_clock::now();
        int is_device = 1;

        #pragma omp parallel for
        for (int i = 0; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif

            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            p_poisson_noise[i] = 0;

            for (size_t j = 0; j < Testing::g_repeat_iteration; ++j)
            {
                p_poisson_noise[i] += generatePoissonNoise(i, z1, z2, z3, z4, 50);
            }

            p_poisson_noise[i] /= Testing::g_repeat_iteration;
        }
        stop_poisson_noise_openmp_cpu = high_resolution_clock::now();

        for (int i = 0; i < max_size; ++i)
        {
            unsigned int random_value = p_poisson_noise[i];
            float absolute_error = std::abs(static_cast<float>(random_value) - static_cast<float>(i));
            float relative_error = 0.0F;

            if (i > 0)
            {
                relative_error = 100.0F * absolute_error / static_cast<float>(i);
            }

      //      std::cout << i << "\t" << random_value << "\t" << absolute_error << "\t" << relative_error << std::endl;

            if (Testing::g_repeat_iteration > 5000)
            {
                ASSERT_EQ(absolute_error < 5.0, true);
                ASSERT_EQ(relative_error < 1.5, true);
            }
        }
        std::cout << "Running (Poisson) on " <<(is_device == 1 ? "CPU" : "GPU") << std::endl;
    }

    // GPU with OpenMP
    {
        start_poisson_noise_openmp_gpu = high_resolution_clock::now();
        int is_device = 1;

        #ifdef HAS_OPENMP_GPU
        #pragma omp target teams distribute parallel for map(to:p_gaussian_noise[0:max_size]) map(from:p_poisson_noise[0:max_size], is_device) 
        #else
        #pragma omp parallel for
        #endif
        for (int i = 0; i < max_size; ++i)
        {
            #ifdef HAS_OPENMP_GPU
            is_device = omp_is_initial_device();
            #endif
            
            unsigned int z1 = getSeed(i * 4 + 0);
            unsigned int z2 = getSeed(i * 4 + 1);
            unsigned int z3 = getSeed(i * 4 + 2);
            unsigned int z4 = getSeed(i * 4 + 3);

            p_poisson_noise[i] = 0;

            for (size_t j = 0; j < Testing::g_repeat_iteration; ++j)
            {
                p_poisson_noise[i] += generatePoissonNoise(p_gaussian_noise[i], z1, z2, z3, z4, 50);
            }

            p_poisson_noise[i] /= Testing::g_repeat_iteration;
        }
        stop_poisson_noise_openmp_gpu = high_resolution_clock::now();

        for (int i = 0; i < max_size; ++i)
        {
            unsigned int random_value = p_poisson_noise[i];
	        float absolute_error = std::abs(static_cast<float>(random_value) - static_cast<float>(i));
            float relative_error = 0.0F;

            if (i > 0)
            {
                relative_error = 100.0F * absolute_error / static_cast<float>(i);
            }

            if (Testing::g_repeat_iteration > 5000)
            {
                ASSERT_EQ(absolute_error < 5.0F, true);
                ASSERT_EQ(relative_error < 1.5F, true);
            }
        }
        std::cout << "Running (Poisson) on " << (is_device == 1 ? "CPU" : "GPU") << std::endl;
    }

    poisson_noise_image.saveTIFF("poisson_noise.tif");


    std::cout << "Poisson noise\tSerial\tCPU\t" << duration_cast<milliseconds>(stop_poisson_noise_cpu - start_poisson_noise_cpu).count() << std::endl;
    std::cout << "Poisson noise\tOpenMP\tCPU\t" << duration_cast<milliseconds>(stop_poisson_noise_openmp_cpu - start_poisson_noise_openmp_cpu).count() << std::endl;
    std::cout << "Poisson noise\tOpenMP\tGPU\t" << duration_cast<milliseconds>(stop_poisson_noise_openmp_gpu - start_poisson_noise_openmp_gpu).count() << std::endl;
}
