#include "types.h"
#include <immintrin.h>


static void mac_avx_mixed(__m256d& sum, double*& dp, double*& cp)
{
  __m256d d = _mm256_loadu_pd(dp); dp += 4;    //data may be unaligned
  __m256d c = _mm256_load_pd(cp); cp += 4;     //coeffs should be aligned
  sum = _mm256_add_pd(sum, _mm256_mul_pd(c,d));
}

static void mac_avx_unaligned(__m256d& sum, double*& dp, double*& cp)
{
  __m256d d = _mm256_loadu_pd(dp); dp += 4;    //data may be unaligned
  __m256d c = _mm256_loadu_pd(cp); cp += 4;     //coeffs should be aligned
  sum = _mm256_add_pd(sum, _mm256_mul_pd(c,d));
}

#ifdef M32
    #define mac_avx mac_avx_unaligned
#else
    #define mac_avx mac_avx_mixed
#endif

static const int UNROLLS = 2;


longSample convolve_avx(sample* data, sample* coeffs, int len, int last_idx)
{
    double a = 0.0;

    int lmt = len - last_idx;

    double *dp = data + last_idx;
    double *cp = coeffs;

    __m256d sum[UNROLLS];

    for (int i = 0; i < UNROLLS; ++i)
    	sum[i] = _mm256_setzero_pd();
    
    int cnt;
    for (cnt = lmt; cnt >= UNROLLS * 4; cnt -= UNROLLS * 4)
    {
    	for (int i = 0; i < UNROLLS; ++i)
      		mac_avx_unaligned(sum[i], dp, cp);      //passed and returned by reference!
    }

   	while (cnt--)
   	{
   		a += *dp++ * *cp++;
   	}
    
    dp = data;
    for (cnt = last_idx; cnt >= UNROLLS * 4; cnt -= UNROLLS * 4)
    {
      for (int i = 0; i < UNROLLS; ++i)
      		mac_avx_unaligned(sum[i], dp, cp);      //passed and returned by reference!
    }
    
    while (cnt--)
   	{
   		a += *dp++ * *cp++;
   	}
    
    double sse_sums[4] __attribute__ ((aligned (32)));      //aligned in the stack

    for (int i = 0; i < UNROLLS; ++i)
   	{ 	
    	_mm256_store_pd(sse_sums, sum[i]);
    	a += sse_sums[0] + sse_sums[1] + sse_sums[2] + sse_sums[3];
    }

    return a;
}


RegisterImplementation ravx1("CAVX", convolve_avx, 4*UNROLLS, false, false);
