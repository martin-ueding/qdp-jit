#ifndef QDP_FLOAT_COMPRESS_H
#define QDP_FLOAT_COMPRESS_H

namespace QDP {

unsigned char ieee754_64_compress(double in_flt , size_t mant_count , unsigned int bias);
unsigned char ieee754_32_compress(float in_flt  , size_t mant_count , unsigned char bias);

double ieee754_64_uncompress( unsigned char in , size_t mant_count , unsigned int bias);
float  ieee754_32_uncompress( unsigned char in , size_t mant_count , unsigned char bias);

}

#endif
