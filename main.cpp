#include <vector>
#include <random>
#include <algorithm>
#include <array>
#include <cstdio>

#include "timediff.h"
#include "types.h"

using namespace std;


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

    	for (auto& impl: Implementations::getInstance().getImplementations())
    	{
    		timediff td;

	    	longSample result = 0.0;

	    	for (int j = 0; j < iterations; ++j)
		    	for (int i = 0; i < t.data.size(); ++i)
					result += impl.function(t.data.data(), t.coeffs.data(), t.data.size(), i);
			
			double t = td.get_diff();
		
			printf("%s:\tUnrolled: %d SR: %d ForcedLong: %d\n", impl.name.c_str(), impl.unrolled, impl.separated, impl.forcedLong);
			printf("\tTime: % 3.4f\tMMACs/s: % 9.3f\tResult: % 30.15Lf\n", t, MACs / t / 1e6, result);
    	}
}
    	
		