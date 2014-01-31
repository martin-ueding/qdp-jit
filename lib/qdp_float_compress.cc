#include "qdp.h"


namespace QDP {


unsigned char ieee754_64_compress(double in_flt,size_t mant_count, unsigned int bias)
{
  ieee754_double in;
  in.d = in_flt;

  int exp_minus = (int)(in.ieee.exponent) - bias;

  if (-exp_minus >= (1 << (8-1-mant_count))) return 0;

  if (-exp_minus <= 0) return 1 | (in.ieee.negative << 7);

  unsigned int ex = -exp_minus;

  unsigned int ret = (in.ieee.negative << 7) | (ex << mant_count) | (in.ieee.mantissa0 >> (20 - mant_count));

  return (unsigned char)ret;
}



double ieee754_64_uncompress( unsigned char in , size_t mant_count , unsigned int bias)
{
  if (!in)
    return 0.0f;

  unsigned int sign = in >> 7;
  unsigned int expo = -((in & 127) >> mant_count) + bias;
  unsigned int mant = (in & ((1 << mant_count)-1)) << (20 - mant_count);

  ieee754_double fl;

  fl.ieee.negative = sign;
  fl.ieee.exponent = expo;
  fl.ieee.mantissa0 = mant;
  fl.ieee.mantissa1 = 0;

  return fl.d;
}




unsigned char ieee754_32_compress(float in_flt,size_t mant_count, unsigned char bias)
{
  ieee754_float in;
  in.f = in_flt;

  char exp_minus = (char)(in.ieee.exponent) - bias;

  // underflow
  if (-exp_minus >= (1 << (8-1-mant_count))) return 0;

  // overflow
  if (-exp_minus <= 0) return 1 | (in.ieee.negative << 7);

  unsigned int ex = -exp_minus;

  unsigned int ret = (in.ieee.negative << 7) | (ex << mant_count) | (in.ieee.mantissa >> (23 - mant_count));

  if (ret > 255 )
    std::cout << "problem , ret = " << ret << "\n";

  return (unsigned char)ret;
}


float ieee754_32_uncompress( unsigned char in , size_t mant_count , unsigned char bias)
{
  // underflow
  if (!in)
    return 0.0f;

  unsigned int sign = in >> 7;
  unsigned int expo = -((in & 127) >> mant_count) + bias;
  unsigned int mant = (in & ((1 << mant_count)-1)) << (23 - mant_count);

  //printf("uncompress: sign=%d exp=%d man=%d\n",sign,expo,mant);  

  ieee754_float fl;
  fl.ieee.negative = sign;
  fl.ieee.exponent = expo;
  fl.ieee.mantissa = mant;

  return fl.f;
}

}
