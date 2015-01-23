#include "types.h"
#include <immintrin.h>


static inline void mac_sse2_mixed(__m128d& sum, sample *__restrict__ &dp, sample *__restrict__ &cp)
{
  //__m128d d = _mm_lddqu_si128((const __m128i*)dp); dp += 2;    //data may be unaligned  
  __m128d d = _mm_loadu_pd(dp); dp += 2;    //data may be unaligned
  __m128d c = _mm_load_pd(cp); cp += 2;     //coeffs should be aligned
  sum = _mm_add_pd(sum, _mm_mul_pd(c,d));
}

static inline void mac_sse2_unaligned(__m128d& sum, sample *__restrict__ &dp, sample *__restrict__ &cp)
{
   __m128d c = _mm_loadu_pd(cp); cp += 2;
    __m128d d = _mm_loadu_pd(dp); dp += 2;
    sum = _mm_add_pd(sum, _mm_mul_pd(c,d));
}

static inline void mac_sse2_aligned(__m128d& sum, sample *__restrict__ &dp, sample *__restrict__ &cp)
{
    __m128d c = _mm_load_pd(cp); cp += 2;
    __m128d d = _mm_load_pd(dp); dp += 2;
    sum = _mm_add_pd(sum, _mm_mul_pd(c,d));
}

#define mac_sse2 mac_sse2_unaligned

static const int UNROLLS = 4;

longSample convolve_sse2(sample *__restrict__ data, sample *__restrict__ coeffs, int len, int last_idx)
{
    sample a = 0;

    sample *dp = data + last_idx;
    sample *cp = coeffs;

    int lmt = len - last_idx;

    __m128d sums[UNROLLS];

    for (auto& s: sums)
        s = _mm_setzero_pd();

    int cnt;
    for (cnt = lmt; cnt >= UNROLLS*2; cnt -= UNROLLS*2)
    {
        for (auto& s: sums)
            mac_sse2(s, dp, cp);		//passed and returned by reference!
    }

    while (cnt--)
    {
        a = *dp++ * *cp++;
    }
    
    //start from the start of the buffer
    dp = data;    
    //cp = coeffs + lmt;        
    for (cnt = last_idx; cnt >= UNROLLS*2; cnt -= UNROLLS*2)
    {
        for (auto& s: sums)
            mac_sse2(s, dp, cp);        //passed and returned by reference!
    }

    //last iteration should include single access
    while (cnt--)
    {
        a += *dp++ * *cp++;
    }
    
    sample sse_sums[2] __attribute__ ((aligned (16)));      //aligned in the stack


    for (auto& s: sums)
    {
        _mm_store_pd(sse_sums, s);
        a += sse_sums[0] + sse_sums[1];
    }

    return a;
}

RegisterImplementation rsse2_1("SSE2", convolve_sse2, 2, false, false);