#ifndef QDP_FLOAT_COMPRESS_H
#define QDP_FLOAT_COMPRESS_H

namespace QDP {

namespace FLOAT {
  extern size_t mant_count_sp;
  extern size_t mant_count_dp;

  extern unsigned char bias_sp;
  extern unsigned int  bias_dp;
}

unsigned char ieee754_64_compress(double in_flt);
unsigned char ieee754_32_compress(float in_flt);

double ieee754_64_uncompress( unsigned char in);
float  ieee754_32_uncompress( unsigned char in);

}

#endif
