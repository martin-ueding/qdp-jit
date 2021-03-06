// $Id: generic_blas_vadd3_g5.h,v 1.2 2007-06-10 14:32:10 edwards Exp $

/*! @file
 *  @brief Generic Scalar VAXPY routine
 *
 */

#ifndef QDP_GENERIC_BLAS_VADD_G5
#define QDP_GENERIC_BLAS_VADD_G5

namespace QDP {

// (Vector) out = (Vector) X + (Vector) P{+} Y
inline
void add_g5ProjPlus(REAL *Out, REAL *X, REAL *Y, int n_4vec)
{
  register double x0r;
  register double x0i;
  
  register double x1r;
  register double x1i;
  
  register double x2r;
  register double x2i;
  
  register double y0r;
  register double y0i;
  
  register double y1r;
  register double y1i;
  
  register double y2r;
  register double y2i;
  
  register double z0r;
  register double z0i;
  
  register double z1r;
  register double z1i;
  
  register double z2r;
  register double z2i;
  
  
  register int index_x = 0;
  register int index_y = 0;
  register int index_z = 0;
  
  register int counter;
  
  for( counter = 0; counter < n_4vec; counter++) {
    // Spin Component 0 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r + y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i + y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r + y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i + y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r + y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i + y2i;  
    Out[index_z++] = (REAL)z2i;

    // Spin Component 1 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r + y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i + y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r + y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i + y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r + y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i + y2i;  
    Out[index_z++] = (REAL)z2i;

    index_y +=12; // Skip 2 color vecs (3x2x2=12)
    
    // 2 spin components: Copy...

    x0r = (double)X[index_x++];
    x0i = (double)X[index_x++];  
    x1r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];   
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];   
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i; 
    x0r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];  
    Out[index_z++] =(REAL) x2i;
    x1r = (double)X[index_x++];
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];      
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i;
   
    // Cache pulls
    x0r = (double)X[index_x];
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x+1];
    Out[index_z++] =(REAL) x2i;

    
  }
}

// (Vector) out = (Vector) X + (Vector) P{-} Y
inline
void add_g5ProjMinus(REAL *Out, REAL *X, REAL *Y,int n_4vec)
{
  register double x0r;
  register double x0i;
  
  register double x1r;
  register double x1i;
  
  register double x2r;
  register double x2i;
  
  register double y0r;
  register double y0i;
  
  register double y1r;
  register double y1i;
  
  register double y2r;
  register double y2i;
  
  register double z0r;
  register double z0i;
  
  register double z1r;
  register double z1i;
  
  register double z2r;
  register double z2i;
  
  register int index_x = 0;
  register int index_y = 0;
  register int index_z = 0;
  
  register int counter;
  
  for( counter = 0; counter < n_4vec; counter++) {
    index_y += 12;

    // 2 spin components: Copy...
    x0r = (double)X[index_x++];
    x0i = (double)X[index_x++];  
    x1r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];   
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];   
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i; 
    x0r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];  
    Out[index_z++] =(REAL) x2i;
    x1r = (double)X[index_x++];
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];      
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i;
   
    // Prefetch
    x0r = (double)X[index_x++];
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];
    Out[index_z++] =(REAL) x2i;

    // Spin Component 0 (AYPX)
    y0r = (double)Y[index_y++];
    z0r = x0r + y0r;
    Out[index_z++] =(REAL) z0r;
  
    y0i = (double)Y[index_y++];
    z0i = x0i + y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r + y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i + y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r + y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i + y2i;  
    Out[index_z++] = (REAL)z2i;

    // Spin Component 1 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r + y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i + y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r + y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i + y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r + y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i + y2i;  
    Out[index_z++] = (REAL)z2i;


  }
}


// (Vector) out = (Vector) X - (Vector) P{+} Y 
inline
void sub_g5ProjPlus(REAL *Out, REAL *X, REAL *Y,int n_4vec)
{
  register double x0r;
  register double x0i;
  
  register double x1r;
  register double x1i;
  
  register double x2r;
  register double x2i;
  
  register double y0r;
  register double y0i;
  
  register double y1r;
  register double y1i;
  
  register double y2r;
  register double y2i;
  
  register double z0r;
  register double z0i;
  
  register double z1r;
  register double z1i;
  
  register double z2r;
  register double z2i;
  
  register int index_x = 0;
  register int index_y = 0;
  register int index_z = 0;
  
  register int counter;
  
  for( counter = 0; counter < n_4vec; counter++) {
    // Spin Component 0 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r - y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i - y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r - y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i - y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r - y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i - y2i;  
    Out[index_z++] = (REAL)z2i;

    // Spin Component 1 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r - y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i - y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r - y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i - y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r - y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i - y2i;  
    Out[index_z++] = (REAL)z2i;

    index_y +=12; // Skip 2 color vecs (3x2x2=12)
    
    // 2 spin components: Copy...

    x0r = (double)X[index_x++];
    x0i = (double)X[index_x++];  
    x1r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];   
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];   
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i; 
    x0r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];  
    Out[index_z++] =(REAL) x2i;
    x1r = (double)X[index_x++];
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];      
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i;
   
    // Cache pulls
    x0r = (double)X[index_x];
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x+1];
    Out[index_z++] =(REAL) x2i;

    
  }
}

// (Vector) out = (Vector) X - (Vector) P{-} Y
inline
void sub_g5ProjMinus(REAL *Out, REAL *X, REAL *Y,int n_4vec)
{
  register double x0r;
  register double x0i;
  
  register double x1r;
  register double x1i;
  
  register double x2r;
  register double x2i;
  
  register double y0r;
  register double y0i;
  
  register double y1r;
  register double y1i;
  
  register double y2r;
  register double y2i;
  
  register double z0r;
  register double z0i;
  
  register double z1r;
  register double z1i;
  
  register double z2r;
  register double z2i;
  
  
  register int index_x = 0;
  register int index_y = 0;
  register int index_z = 0;
  
  register int counter;
  
  for( counter = 0; counter < n_4vec; counter++) {
    index_y += 12;

    // 2 spin components: Copy...
    x0r = (double)X[index_x++];
    x0i = (double)X[index_x++];  
    x1r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];   
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];   
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i; 
    x0r = (double)X[index_x++];    
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];  
    Out[index_z++] =(REAL) x2i;
    x1r = (double)X[index_x++];
    Out[index_z++] =(REAL) x0r;
    x1i = (double)X[index_x++];    
    Out[index_z++] =(REAL) x0i;
    x2r = (double)X[index_x++];      
    Out[index_z++] =(REAL) x1r;
    x2i = (double)X[index_x++];
    Out[index_z++] =(REAL) x1i;
   
    // Prefetch
    x0r = (double)X[index_x++];
    Out[index_z++] =(REAL) x2r;
    x0i = (double)X[index_x++];
    Out[index_z++] =(REAL) x2i;

    // Spin Component 0 (AYPX)
    y0r = (double)Y[index_y++];
    z0r = x0r - y0r;
    Out[index_z++] =(REAL) z0r;
  
    y0i = (double)Y[index_y++];
    z0i = x0i - y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r - y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i - y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r - y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i - y2i;  
    Out[index_z++] = (REAL)z2i;

    // Spin Component 1 (AYPX)
    x0r = (double)X[index_x++];
    y0r = (double)Y[index_y++];
    z0r = x0r - y0r;
    Out[index_z++] =(REAL) z0r;
  
    x0i = (double)X[index_x++];  
    y0i = (double)Y[index_y++];
    z0i = x0i - y0i;
    Out[index_z++] =(REAL) z0i;

    x1r = (double)X[index_x++];    
    y1r = (double)Y[index_y++];
    z1r = x1r - y1r;
    Out[index_z++] = (REAL)z1r;

    x1i = (double)X[index_x++];    
    y1i = (double)Y[index_y++];
    z1i = x1i - y1i;
    Out[index_z++] = (REAL)z1i;
    
    x2r = (double)X[index_x++];
    y2r = (double)Y[index_y++];     
    z2r = x2r - y2r;
    Out[index_z++] = (REAL)z2r;
   
    x2i = (double)X[index_x++]; 
    y2i = (double)Y[index_y++];
    z2i = x2i - y2i;  
    Out[index_z++] = (REAL)z2i;


  }
}




} // namespace QDP;

#endif // guard
