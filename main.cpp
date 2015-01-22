#include <vector>
#include <random>
#include <algorithm>
#include <array>
#include <cstdio>

#include "timediff.h"
#include "types.h"

using namespace std;


longSample conv_split_unrolled4_sep_result(sample* data, sample* coeffs, int len, int last_idx)
{      
    sample *dp = data;
    sample *cp = coeffs;
    longSample a[4] = {0.0, 0.0, 0.0, 0.0};
   
    dp = data+last_idx;
    int cnt = len - last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++;
        a[1] += (longSample)*dp++ * (longSample)*cp++;
        a[2] += (longSample)*dp++ * (longSample)*cp++;
        a[3] += (longSample)*dp++ * (longSample)*cp++;
    }   

    while (cnt--)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++; 
    }

    
    dp = data;
    cnt = last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++;
        a[1] += (longSample)*dp++ * (longSample)*cp++;
        a[2] += (longSample)*dp++ * (longSample)*cp++;
        a[3] += (longSample)*dp++ * (longSample)*cp++;
    }

    while (cnt--)
    {
        a[1] += (longSample)*dp++ * (longSample)*cp++; 
    }

    return a[0] + a[1] + a[2] + a[3];
}

longSample conv_split_unrolled4_sep_result_no_long(sample* data, sample* coeffs, int len, int last_idx)
{      
    sample *dp = data;
    sample *cp = coeffs;
    longSample a[4] = {0.0, 0.0, 0.0, 0.0};
   
    dp = data+last_idx;
    int cnt = len - last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a[0] += *dp++ * *cp++;
        a[1] += *dp++ * *cp++;
        a[2] += *dp++ * *cp++;
        a[3] += *dp++ * *cp++;
    }   

    while (cnt--)
    {
        a[0] += *dp++ * *cp++; 
    }

    
    dp = data;
    cnt = last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a[0] += *dp++ * *cp++;
        a[1] += *dp++ * *cp++;
        a[2] += *dp++ * *cp++;
        a[3] += *dp++ * *cp++;
    }

    while (cnt--)
    {
        a[1] += *dp++ * *cp++; 
    }

    return a[0] + a[1] + a[2] + a[3];
}

longSample conv_split_unrolled2_sep_result(sample* data, sample* coeffs, int len, int last_idx)
{      
    sample *dp = data;
    sample *cp = coeffs;
    longSample a[2] = {0.0, 0.0};
   
    dp = data+last_idx;
    int cnt = len - last_idx;
    for (; cnt >= 2; cnt -= 2)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++;
        a[1] += (longSample)*dp++ * (longSample)*cp++;
    }   

    if (cnt)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++; 
    }

    
    dp = data;
    cnt = last_idx;
    for (; cnt >= 2; cnt -= 2)
    {
        a[0] += (longSample)*dp++ * (longSample)*cp++;
        a[1] += (longSample)*dp++ * (longSample)*cp++;
    }

    if (cnt)
    {
        a[1] += (longSample)*dp++ * (longSample)*cp++; 
    }

    return a[0] + a[1];
}


longSample conv_split_unrolled4_no_sep_result(sample* data, sample* coeffs, int len, int last_idx)
{      
    sample *dp = data;
    sample *cp = coeffs;
    longSample a = 0.0;
   
    dp = data+last_idx;
    int cnt = len - last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
    }   

    while (cnt--)
    {
        a += (longSample)*dp++ * (longSample)*cp++; 
    }

    
    dp = data;
    cnt = last_idx;
    for (; cnt >= 4; cnt -= 4)
    {
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
        a += (longSample)*dp++ * (longSample)*cp++;
    }

    while (cnt--)
    {
        a += (longSample)*dp++ * (longSample)*cp++; 
    }

    return a;
}

longSample conv_split(sample* data, sample* coeffs, int len, int last_idx)
{      
    sample *dp = data;
    sample *cp = coeffs;
    longSample a = 0.0;
   
    dp = data+last_idx;
    int cnt = len - last_idx;
    for (; cnt >= 1; cnt -= 1)
    {
        a += (longSample)*dp++ * (longSample)*cp++;
    }   
 
    dp = data;
    cnt = last_idx;
    for (; cnt >= 1; cnt -= 1)
    {
        a += (longSample)*dp++ * (longSample)*cp++;   
    }

    return a;
}

longSample conv_dummy_FPU(sample* data, sample* coeffs, int len, int last_idx)
{
    longSample a = 0;
    sample* dp = data;
    sample* cp = coeffs;

    int data_idx = last_idx;

    for (int i = 0; i < len; ++i)
    {
        a += (longSample)dp[data_idx] * (longSample)cp[i];
		if (data_idx >= len)
			data_idx = 0;
    }

    return a;
}

longSample conv_dummy(sample* data, sample* coeffs, int len, int last_idx)
{
	longSample a = 0;
    sample* dp = data;
    sample* cp = coeffs;

    int data_idx = last_idx;

    for (int i = 0; i < len; ++i)
    {
        a += dp[data_idx] * cp[i];
		if (data_idx >= len)
			data_idx = 0;
    }

    return a;
}

struct TestData
{
    TestData(size_t noOfSamples): data(noOfSamples, 0), coeffs(noOfSamples, 0.0) 
    {
    	mt19937 mt;
    	uniform_real_distribution<sample> rng(-1.0, 1.0);
    	for_each(data.begin(), data.end(), [&](sample& x) {x = rng(mt);});
    	for_each(coeffs.begin(), coeffs.end(), [&](sample& x) {x = rng(mt);});	
    }
    std::vector<sample> data;
    std::vector<sample> coeffs;
};

struct implementation
{
	const std::string name;
	const convFun fun;
};

std::vector<implementation> implementations =
{
	{"Simple", 						conv_dummy},
	{"SplitUnrolled4SepResults",	conv_split_unrolled4_sep_result},
	{"SplitUnrolled2SepResults",	conv_split_unrolled2_sep_result},
	{"SplitUnrolled4NoSepResults",  conv_split_unrolled4_no_sep_result},
	{"SimpleForceFPU",	 			conv_dummy_FPU},
	{"SimpleSplit", 				conv_split},
	{"SplitUnrolled4SepResultsNoLong", conv_split_unrolled4_sep_result_no_long}
};

const int iterations = 10000;
const int coeffs = 499;
double MACs = iterations * (double)coeffs * (double)coeffs;

void printInfo()
{
	if (sizeof(int*) == 4)
		printf("Mode: 32b\n");
	if (sizeof(int*) == 8)
		printf("Mode: 64b\n");

	printf("sizeof(sample) = %d, sizeof(longSample) = %d\n", (int)sizeof(sample), (int)sizeof(longSample));

	printf("Iterations: %d\n", iterations);
	printf("Coeffs:     %d\n", coeffs);
	printf("MMACs:      %9.3f\n", MACs / 1e6);
}

int main()
{
		printInfo();
    	TestData t(coeffs);

    	for (auto& impl: implementations)
    	{
    		timediff td;

	    	longSample result = 0.0;

	    	for (int j = 0; j < iterations; ++j)
		    	for (int i = 0; i < t.data.size(); ++i)
					result += impl.fun(t.data.data(), t.coeffs.data(), t.data.size(), i);
			
			double t = td.get_diff();
			double MACs = iterations * (double)coeffs * (double)coeffs;

			printf("%s:\n\tTime: % 3.4f\tMMACs/s: % 9.3f\tResult: % 30.15Lf\n", impl.name.c_str(), t, MACs / t / 1e6, result);
    	}
}
    	
		