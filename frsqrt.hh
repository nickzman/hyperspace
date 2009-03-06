#ifndef __FRSQRT_HH
#define __FRSQRT_HH
//
// Functions for the fast computation of reciprocal square roots
// by means of Newton iteration, e.g. compute a good initial
// guess and iterate up to desired precision.
//
// Optimisations for/by
//     Altivec   : USE_RSQRT_ALTIVEC
//     SSE/SSE2  : USE_RSQRT_SSE / USE_RSQRT_SSE2
//     Lookup    : Lookup table approach by Ken Turkowski
//                 USE_RSQRT_LOOKUP
//     Magic No. : Magic numbers for initial guess
//                 USE_RSQRT_MAGIC
//     Fallback  : standard implementation via libm
//                 USE_RSQRT_LIBM
//
// If no manual override is specified, the optimisation is
// chosen automatically depending on available compiler.
//
// To initialise RSQRT, call init_rsqrt() first. Afterwards
// "rsqrtf" and "rsqrt" compute the reciprocal square root.
//

// manual override
//#define USE_RSQRT_LOOKUP

// define to minimise FPU usage
#define PURE_VECTOR

//
// automatically decide what specific implementation to use
// if not chosen before
// 
#if !defined(USE_RSQRT_ALTIVEC) && !defined(USE_RSQRT_SSE2)   && \
    !defined(USE_RSQRT_SSE)     && !defined(USE_RSQRT_LOOKUP) && \
    !defined(USE_RSQRT_MAGIC)   && !defined(USE_RSQRT_LIBM)
        
#if defined __GNUC__ && defined __APPLE_ALTIVEC__
#  define USE_RSQRT_ALTIVEC
#elif defined __GNUC__ && defined __SSE__
#  if defined __SSE2__
#    define USE_RSQRT_SSE2
#  else
#    define USE_RSQRT_SSE
#  endif
#else
#  define USE_RSQRT_LIBM
#endif
        
#endif

//////////////////////////////////////////////////////
//
// GCC and Altivec
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_ALTIVEC
#undef USE_RSQRT_ALTIVEC

#include <ppc_intrinsics.h>

inline float
rsqrtf ( float x )
{
    float r = __frsqrtes( x );

    r *= ((3.0f - r * r * x) * 0.5f);
    r *= ((3.0f - r * r * x) * 0.5f);
    
    return r;
}

inline double
rsqrt ( double x )
{
    double r = __frsqrte( x );

    r *= ((3.0 - r * r * x) * 0.5);
    r *= ((3.0 - r * r * x) * 0.5);
    r *= ((3.0 - r * r * x) * 0.5);
    
    return r;
}

// dummy routine
inline void init_rsqrt () {}

#endif  // USE_RSQRT_ALTIVEC

//////////////////////////////////////////////////////
//
// GCC / ICC with SSE
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_SSE
#undef USE_RSQRT_SSE

#include <xmmintrin.h>

const __m128  f3  = _mm_set_ss(3.0f);  // 3 as SSE value
const __m128  f05 = _mm_set_ss(0.5f);  // 0.5 as SSE value

inline float
rsqrtf ( float x )
{
#ifdef PURE_VECTOR
    __m128  xx = _mm_load_ss( & x );
    __m128  xr = _mm_rsqrt_ss( xx );
    __m128  xt;

    xt = _mm_mul_ss( xr, xr );
    xt = _mm_mul_ss( xt, xx );
    xt = _mm_sub_ss( f3, xt );
    xt = _mm_mul_ss( xt, f05 );
    xr = _mm_mul_ss( xr, xt );

    _mm_store_ss( & x, xr );

    return x;
#else
    float   r;

    _mm_store_ss( & r, _mm_rsqrt_ss( _mm_load_ss( & x ) ) );

    r *= ((3.0f - r * r * x) * 0.5f);
    
    return r;
#endif
}

inline double
rsqrt ( double x )
{
    float arg = x;

    _mm_store_ss( & arg, _mm_rsqrt_ss( _mm_load_ss( & arg ) ) );

    double  r = arg;
    
    r *= ((3.0 - r * r * x) * 0.5);
    r *= ((3.0 - r * r * x) * 0.5);

    return r;
}

// dummy routine
inline void init_rsqrt () {}

#endif  // USE_RSQRT_SSE

//////////////////////////////////////////////////////
//
// GCC / ICC with SSE2
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_SSE2
#undef USE_RSQRT_SSE2

#include <emmintrin.h>

const __m128  f3  = _mm_set_ss(3.0f);  // 3 as single precision SSE value
const __m128  f05 = _mm_set_ss(0.5f);  // 0.5 as single precision SSE value

inline float
rsqrtf ( float x )
{
#ifdef PURE_VECTOR
    __m128  xx = _mm_load_ss( & x );
    __m128  xr = _mm_rsqrt_ss( xx );
    __m128  xt;

    xt = _mm_mul_ss( xr, xr );
    xt = _mm_mul_ss( xt, xx );
    xt = _mm_sub_ss( f3, xt );
    xt = _mm_mul_ss( xt, f05 );
    xr = _mm_mul_ss( xr, xt );

    _mm_store_ss( & x, xr );

    return x;
#else
    float   r;

    _mm_store_ss( & r, _mm_rsqrt_ss( _mm_load_ss( & x ) ) );

    r *= ((3.0f - r * r * x) * 0.5f);
    
    return r;
#endif
}

const __m128d  d3  = _mm_set_sd(3.0);  // 3 as double precision SSE value
const __m128d  d05 = _mm_set_sd(0.5);  // 0.5 as double precision SSE value

inline double
rsqrt ( double x )
{
#ifdef PURE_VECTOR
    float    fx = x;
    __m128d  xx = _mm_load_sd( & x );
    __m128d  xr = _mm_cvtss_sd( xx, _mm_rsqrt_ss( _mm_load_ss( & fx ) ) );
    __m128d  xt;

    xt = _mm_mul_sd( xr, xr );
    xt = _mm_mul_sd( xt, xx );
    xt = _mm_sub_sd( d3, xt );
    xt = _mm_mul_sd( xt, d05 );
    xr = _mm_mul_sd( xr, xt );

    xt = _mm_mul_sd( xr, xr );
    xt = _mm_mul_sd( xt, xx );
    xt = _mm_sub_sd( d3, xt );
    xt = _mm_mul_sd( xt, d05 );
    xr = _mm_mul_sd( xr, xt );

    _mm_store_sd( & x, xr );

    return x;
#else    
    float arg = x;

    _mm_store_ss( & arg, _mm_rsqrt_ss( _mm_load_ss( & arg ) ) );

    double  r = arg;
    
    r *= ((3.0 - r * r * x) * 0.5);
    r *= ((3.0 - r * r * x) * 0.5);

    return r;
#endif    
}

// dummy routine
inline void init_rsqrt () {}

#endif  // USE_RSQRT_SSE2

//////////////////////////////////////////////////////
//
// lookup table and floating point arithmetics
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_LOOKUP
#undef USE_RSQRT_LOOKUP

#include <math.h>

typedef unsigned char rsqrt_uchar;
typedef unsigned int  rsqrt_uint;
typedef unsigned long rsqrt_ulong;

// desired precision
const rsqrt_uint PRECISION   = 3;

// Specified parameters
const rsqrt_uint LOOKUP_BITS = 8;                     // Number of mantissa bits for lookup
const rsqrt_uint EXP_POS     = 23;                    // Position of the exponent
const rsqrt_uint EXP_BIAS    = 127;                   // Bias of exponent
// The mantissa is assumed to be just down from the exponent
    
// Derived parameters
const rsqrt_uint LOOKUP_POS  = (EXP_POS-LOOKUP_BITS); // Position of mantissa lookup
const rsqrt_uint SEED_POS    = (EXP_POS-8);           // Position of mantissa seed
const rsqrt_uint TABLE_SIZE  = (2 << LOOKUP_BITS);    // Number of entries in table
const rsqrt_uint LOOKUP_MASK = (TABLE_SIZE - 1);      // Mask for table input

// extract exponent
#define GET_EXP(a)   (((a) >> EXP_POS) & 0xFF)

// set exponent
#define SET_EXP(a)   ((a) << EXP_POS)

// extended mantissa  MSB's
#define GET_E_MANT(a) (((a) >> LOOKUP_POS) & LOOKUP_MASK)

// set mantissa 8 MSB's
#define SET_MANT_SEED(a) (rsqrt_ulong(a) << SEED_POS)

static rsqrt_uchar rsqrt_table[ TABLE_SIZE ];

typedef union {
    rsqrt_uint i;
    float      f;
} float2int;

static inline void
init_rsqrt ()
{
    static bool rsqrt_init = false;

    if ( rsqrt_init )
        return;

    rsqrt_init = true;

    rsqrt_uchar * h = rsqrt_table;
    
    for ( rsqrt_uint f = 0; f < TABLE_SIZE; f++ )
    {
        float2int   fi, fo;
        
        fi.i = ((EXP_BIAS-1) << EXP_POS) | (f << LOOKUP_POS);
        fo.f = float( 1.0f / ::sqrtf(fi.f) );
        
        // rounding
        *h++ = rsqrt_uchar( ((fo.i + (1<<(SEED_POS-2))) >> SEED_POS) & 0xFF );
    }// for

    // special case for 1.0    
    rsqrt_table[ TABLE_SIZE / 2 ] = 0xFF;
}

//
// compute the Inverse Square Root
// of an IEEE single precision floating-point number.
// written by Ken Turkowski.
//
inline float
rsqrtf ( float x )
{
    float2int  seed;
    rsqrt_uint a = reinterpret_cast< float2int * >( & x )->i;
    
    seed.i = (SET_EXP(((3*EXP_BIAS-1) - GET_EXP(a)) >> 1) |
              SET_MANT_SEED(rsqrt_table[GET_E_MANT (a)]));
    
    // seed: accurate to LOOKUP_BITS
    float r = seed.f;
    
    // first iteration: accurate to 2*LOOKUP_BITS
    r *= ((3.0f - r * r * x) * 0.5f);
    
    // second iteration: accurate to 4*LOOKUP_BITS
    if ( PRECISION >= 2 )
        r *= ((3.0f - r * r * x) * 0.5f);
    
    return r;
}

inline double
rsqrt ( double x )
{
    float2int         seed;
    const float       arg = x;
    const rsqrt_uint  a   = reinterpret_cast< const float2int * >( & arg )->i;
    
    
    seed.i = (SET_EXP(((3*EXP_BIAS-1) - GET_EXP(a)) >> 1) |
              SET_MANT_SEED(rsqrt_table[GET_E_MANT (a)]));
    
    // seed: accurate to LOOKUP_BITS
    double  r = seed.f;
    
    // first iteration: accurate to 2*LOOKUP_BITS
    r *= ((3.0 - r * r * x) * 0.5);
    
    // second iteration: accurate to 4*LOOKUP_BITS
    if ( PRECISION >= 2 )
        r *= ((3.0 - r * r * x) * 0.5);
    
    // third iteration: accurate to 8*LOOKUP_BITS
    if ( PRECISION >= 3 )
        r *= ((3.0 - r * r * x) * 0.5);
    
    return r;
}

#endif  // USE_RSQRT_LOOKUP

//////////////////////////////////////////////////////
//
// Using special (magic) constants as first guess
// (as is implemented in Quake 3 and analysed
//  in "Fast Inverse Square Root" by Chris Lomont)
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_MAGIC
#undef USE_RSQRT_MAGIC

inline float
rsqrtf ( float x )
{
    const float xhalf = 0.5f * x;
    int         i     = *(int*) & x;

    i = 0x5f375a86 - ( i >> 1 );
    x = *(float*) & i;
    x = x * ( 1.5f - xhalf * x * x );
    x = x * ( 1.5f - xhalf * x * x );
    x = x * ( 1.5f - xhalf * x * x );

    return x;
}

inline double
rsqrt ( double x )
{
    const double xhalf = 0.5 * x;
    long long    i     = *(long long*) & x;

    i = 0x5fe6ec85e7de30dall - ( i >> 1 );
    x = *(double*) & i;
    x = x * ( 1.5 - xhalf * x * x );
    x = x * ( 1.5 - xhalf * x * x );
    x = x * ( 1.5 - xhalf * x * x );
    x = x * ( 1.5 - xhalf * x * x );

    return x;
}

// dummy routine
inline void init_rsqrt () {}

#endif  // USE_RSQRT_MAGIC

//////////////////////////////////////////////////////
//
// direct computation with libm methods
//
//////////////////////////////////////////////////////

#ifdef USE_RSQRT_LIBM
#undef USE_RSQRT_LIBM

#include <math.h>

inline float
rsqrtf ( float x )
{
    return 1.0 / ::sqrtf( x ); // requires C99
}

inline double
rsqrt ( double x )
{
    return 1.0 / ::sqrt( x );
}

// dummy routine
inline void init_rsqrt () {}

#endif  // USE_RSQRT_LIBM

#endif  // __FRSQRT_HH
