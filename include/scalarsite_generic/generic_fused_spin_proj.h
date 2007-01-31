
QDP_BEGIN_NAMESPACE(QDP);

#include "scalarsite_generic/generic_mv_switchbox.h"


// Type Aliases
typedef PColorVector<RComplex<REAL>, 3> ColVec;
typedef PSpinVector<ColVec, 4> Spin4;   // 4 spinor 
typedef PSpinVector<ColVec, 2> Spin2;   // 2 spinor 

// SU3 Matrix. I want to keep the same levele  of nesting as for the spinor,
// and hence the PScalar here. Need to dereference such a type with .elem()
typedef PColorMatrix<RComplex<REAL>, 3> SU3;          

inline Spin2
operator*(const QDPExpr<UnaryNode<FnAdjoint, 
	                          Reference<QDPType<RComplex<REAL>,SU3 > > >,
                                  PScalar<SU3> 
                       >&l,

	  const QDPExpr<UnaryNode<FnSpinProjectDir0Plus, 
	                          Reference<QDPType<ColVec,PSpinVector<ColVec,4> > >
	                         >, PSpinVector<ColVec,4> 
                       >&r)
{
  Spin2 ret_val;
  QDPIO::cout << "Fred!" << endl;
  return ret_val;
}


#if 0
//-------------------------
// Proj Dir=0 Minus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir0Minus>::Type_t
spinProjectDir0Minus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir0Minus>::Type_t  d;

  /*                              ( 1  0  0 -i)  ( a0 )    ( a0 - i a3 )
   *  B  :=  ( 1 - Gamma  ) A  =  ( 0  1 -i  0)  ( a1 )  = ( a1 - i a2 )
   *                    0         ( 0  i  1  0)  ( a2 )    ( a2 + i a1 )
   *                              ( i  0  0  1)  ( a3 )    ( a3 + i a0 )

   * Therefore the top components are

   *      ( b0r + i b0i )  =  ( {a0r + a3i} + i{a0i - a3r} )
   *      ( b1r + i b1i )     ( {a1r + a2i} + i{a1i - a2r} )

   * The bottom components of be may be reconstructed using the formula

   *      ( b2r + i b2i )  =  ( {a2r - a1i} + i{a2i + a1r} )  =  ( - b1i + i b1r )
   *      ( b3r + i b3i )     ( {a3r - a0i} + i{a3i + a0r} )     ( - b0i + i b0r ) 
   */
  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      + s1.elem(3).elem(col).imag();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      - s1.elem(3).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      + s1.elem(2).elem(col).imag();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      - s1.elem(2).elem(col).real();
  }
  
  return d;
}

//-------------------------
// Proj Dir=0 Plus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir0Plus>::Type_t
spinProjectDir0Plus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir0Plus>::Type_t  d;
  /* 1 + \gamma_0 =  1  0  0  i 
                     0  1  i  0
                     0 -i  1  0
                    -i  0  0  1 
 
   *      ( d0r + i d0i )  =  ( {x0r - x3i} + i{x0i + x3r} )
   *      ( d1r + i d1i )     ( {x1r - x2i} + i{x1i + x2r} )
   */

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      - s1.elem(3).elem(col).imag();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      + s1.elem(3).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      - s1.elem(2).elem(col).imag();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      + s1.elem(2).elem(col).real();
  }
  
  return d;
}

//-------------------------
// Proj Dir=1 Minus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir1Minus>::Type_t
spinProjectDir1Minus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir1Minus>::Type_t  d;

  /* 1 - \gamma_1 =  1  0  0 +1 
                     0  1 -1  0
                     0 -1  1  0
                    +1  0  0  1 
 
   *      ( b0r + i b0i )  =  ( {a0r + a3r} + i{a0i + a3i} )
   *      ( b1r + i b1i )     ( {a1r - a2r} + i{a1i - a2i} )
   */
  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      + s1.elem(3).elem(col).real();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      + s1.elem(3).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      - s1.elem(2).elem(col).real();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      - s1.elem(2).elem(col).imag();
  }
  
  return d;
}


//-------------------------
// Proj Dir=1 Plus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir1Plus>::Type_t
spinProjectDir1Plus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir1Plus>::Type_t  d;
  /* 1 + \gamma_1 =  1  0  0 -1 
                     0  1  1  0
                     0  1  1  0
                    -1  0  0  1 
 
   *      ( b0r + i b0i )  =  ( {a0r - a3r} + i{a0i - a3i} )
   *      ( b1r + i b1i )     ( {a1r + a2r} + i{a1i + a2i} )
   */


  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      - s1.elem(3).elem(col).real();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      - s1.elem(3).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      + s1.elem(2).elem(col).real();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      + s1.elem(2).elem(col).imag();
  }
  
  return d;
}


//-------------------------
// Proj Dir=2 Minus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir2Minus>::Type_t
spinProjectDir2Minus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir2Minus>::Type_t  d;

  /* 1 - \gamma_2 =  1  0  -i  0 
                     0  1  0  +i
                    +i  0  1   0
                     0 -i  0   1 


   *      ( b0r + i b0i )  =  ( {a0r + a2i} + i{a0i - a2r} )
   *      ( b1r + i b1i )     ( {a1r - a3i} + i{a1i + a3r} )
   */

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      + s1.elem(2).elem(col).imag();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      - s1.elem(2).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      - s1.elem(3).elem(col).imag();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      + s1.elem(3).elem(col).real();
  }
  
  return d;
}


//-------------------------
// Proj Dir=2 Plus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir2Plus>::Type_t
spinProjectDir2Plus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir2Plus>::Type_t  d;

  /* 1 + \gamma_2 =  1  0  i  0 
                     0  1  0 -i
                    -i  0  1  0
                     0  i  0  1 


   *      ( b0r + i b0i )  =  ( {a0r - a2i} + i{a0i + a2r} )
   *      ( b1r + i b1i )     ( {a1r + a3i} + i{a1i - a3r} )
   */


  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      - s1.elem(2).elem(col).imag();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      + s1.elem(2).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      + s1.elem(3).elem(col).imag();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      - s1.elem(3).elem(col).real();
  }
  
  return d;
}

//-------------------------
// Proj Dir=3 Minus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir3Minus>::Type_t
spinProjectDir3Minus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir3Minus>::Type_t  d;

  /* 1 - \gamma_3 =  1  0  -1  0 
                     0  1  0  -1
                    -1  0  1  0
                     0 -1  0  1 

   *      ( b0r + i b0i )  =  ( {a0r - a2r} + i{a0i - a2i} )
   *      ( b1r + i b1i )     ( {a1r - a3r} + i{a1i - a3i} )
   */

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      - s1.elem(2).elem(col).real();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      - s1.elem(2).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      - s1.elem(3).elem(col).real();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      - s1.elem(3).elem(col).imag();
  }
  
  return d;
}


//-------------------------
// Proj Dir=3 Plus
//-------------------------
template<>
inline UnaryReturn<Spin4, FnSpinProjectDir3Plus>::Type_t
spinProjectDir3Plus(const Spin4& s1)
{
  UnaryReturn<Spin4, FnSpinProjectDir3Plus>::Type_t  d;

  /* 1 + \gamma_3 =  1  0  1  0 
                     0  1  0  1
                     1  0  1  0
                     0  1  0  1 

   *      ( b0r + i b0i )  =  ( {a0r + a2r} + i{a0i + a2i} )
   *      ( b1r + i b1i )     ( {a1r + a3r} + i{a1i + a3i} )
   */

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() 
      = s1.elem(0).elem(col).real() 
      + s1.elem(2).elem(col).real();
  
    d.elem(0).elem(col).imag()  
      = s1.elem(0).elem(col).imag() 
      + s1.elem(2).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() 
      = s1.elem(1).elem(col).real() 
      + s1.elem(3).elem(col).real();
  
    d.elem(1).elem(col).imag()  
      = s1.elem(1).elem(col).imag() 
      + s1.elem(3).elem(col).imag();
  }
  
  return d;
}

template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir0Minus>::Type_t
spinReconstructDir0Minus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir0Minus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }

  /*                              ( 1  0  0 -i)  ( a0 )    ( a0 - i a3 )
   *  B  :=  ( 1 - Gamma  ) A  =  ( 0  1 -i  0)  ( a1 )  = ( a1 - i a2 )
   *                    0         ( 0  i  1  0)  ( a2 )    ( a2 + i a1 )
   *                              ( i  0  0  1)  ( a3 )    ( a3 + i a0 )

   * The bottom components of be may be reconstructed using the formula

   *   ( b2r + i b2i )  =  ( {a2r - a1i} + i{a2i + a1r} )  =  ( - b1i + i b1r )
   *   ( b3r + i b3i )     ( {a3r - a0i} + i{a3i + a0r} )     ( - b0i + i b0r ) 
   */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() = -s1.elem(1).elem(col).imag();
    d.elem(2).elem(col).imag() =  s1.elem(1).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() = -s1.elem(0).elem(col).imag();
    d.elem(3).elem(col).imag() =  s1.elem(0).elem(col).real();
  }
 

  return d;
}

template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir0Plus>::Type_t
spinReconstructDir0Plus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir0Plus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }

   /* 1 + \gamma_0 =  1  0  0  i 
                     0  1  i  0
                     0 -i  1  0
                    -i  0  0  1 
 
    *  ( b2r + i b2i )  =  ( {a2r + a1i} + i{a2i - a1r} )  =  ( b1i - i b1r )
    *  ( b3r + i b3i )     ( {a3r + a0i} + i{a3i - a0r} )     ( b0i - i b0r ) 
   */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() =  s1.elem(1).elem(col).imag();
    d.elem(2).elem(col).imag() = -s1.elem(1).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() =  s1.elem(0).elem(col).imag();
    d.elem(3).elem(col).imag() = -s1.elem(0).elem(col).real();
  }
 

  return d;
}


template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir1Minus>::Type_t
spinReconstructDir1Minus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir1Minus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }

  /*                              ( 1  0  0  1)  ( a0 )    ( a0 + a3 )
   *  B  :=  ( 1 - Gamma  ) A  =  ( 0  1 -1  0)  ( a1 )  = ( a1 - a2 )
   *                    1         ( 0 -1  1  0)  ( a2 )    ( a2 - a1 )
   *                              ( 1  0  0  1)  ( a3 )    ( a3 + a0 )
	 
   * The bottom components of be may be reconstructed using the formula

   *  ( b2r + i b2i )  =  ( {a2r - a1r} + i{a2i - a1i} )  =  ( - b1r - i b1i )
   *  ( b3r + i b3i )     ( {a3r + a0r} + i{a3i + a0i} )     (   b0r + i b0i ) 
   */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() = -s1.elem(1).elem(col).real();
    d.elem(2).elem(col).imag() = -s1.elem(1).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() =  s1.elem(0).elem(col).real();
    d.elem(3).elem(col).imag() =  s1.elem(0).elem(col).imag();
  }
 

  return d;
}

template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir1Plus>::Type_t
spinReconstructDir1Plus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir1Plus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }
  /* 1 + \gamma_1 =  1  0  0 -1 
                     0  1  1  0
                     0  1  1  0
                    -1  0  0  1 
 

   *   ( b2r + i b2i )  =  ( {a2r + a1r} + i{a2i + a1i} )  =  (   b1r + i b1i )
   *   ( b3r + i b3i )     ( {a3r - a0r} + i{a3i - a0i} )     ( - b0r - i b0i ) 
  
  */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() =  s1.elem(1).elem(col).real();
    d.elem(2).elem(col).imag() =  s1.elem(1).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() = -s1.elem(0).elem(col).real();
    d.elem(3).elem(col).imag() = -s1.elem(0).elem(col).imag();
  }
 

  return d;
}


template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir2Minus>::Type_t
spinReconstructDir2Minus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir2Minus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }

  /*                              ( 1  0 -i  0)  ( a0 )    ( a0 - i a2 )
   *  B  :=  ( 1 - Gamma  ) A  =  ( 0  1  0  i)  ( a1 )  = ( a1 + i a3 )
   *                    2         ( i  0  1  0)  ( a2 )    ( a2 + i a0 )
   *                              ( 0 -i  0  1)  ( a3 )    ( a3 - i a1 )

   * The bottom components of be may be reconstructed using the formula
   *  ( b2r + i b2i )  =  ( {a2r - a0i} + i{a2i + a0r} )  =  ( - b0i + i b0r )
   *  ( b3r + i b3i )     ( {a3r + a1i} + i{a3i - a1r} )     (   b1i - i b1r )
   */

  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() = -s1.elem(0).elem(col).imag();
    d.elem(2).elem(col).imag() =  s1.elem(0).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() =  s1.elem(1).elem(col).imag();
    d.elem(3).elem(col).imag() = -s1.elem(1).elem(col).real();
  }
 

  return d;
}

template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir2Plus>::Type_t
spinReconstructDir2Plus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir2Plus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }


  /* 1 + \gamma_2 =  1  0  i  0 
                     0  1  0 -i
                    -i  0  1  0
                     0  i  0  1 
		     
   *  ( b2r + i b2i )  =  ( {a2r + a0i} + i{a2i - a0r} )  =  (   b0i - i b0r )
   *  ( b3r + i b3i )     ( {a3r - a1i} + i{a3i + a1r} )     ( - b1i + i b1r ) 
  */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() =  s1.elem(0).elem(col).imag();
    d.elem(2).elem(col).imag() = -s1.elem(0).elem(col).real();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() = -s1.elem(1).elem(col).imag();
    d.elem(3).elem(col).imag() =  s1.elem(1).elem(col).real();
  }
 

  return d;
}


template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir3Minus>::Type_t
spinReconstructDir3Minus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir3Minus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }

  /*                              ( 1  0 -1  0)  ( a0 )    ( a0 - a2 )
   *  B  :=  ( 1 - Gamma  ) A  =  ( 0  1  0 -1)  ( a1 )  = ( a1 - a3 )
   *                    3         (-1  0  1  0)  ( a2 )    ( a2 - a0 )
   *                              ( 0 -1  0  1)  ( a3 )    ( a3 - a1 )
      
   * The bottom components of be may be reconstructed using the formula
   *  ( b2r + i b2i )  =  ( {a2r - a0r} + i{a2i - a0i} )  =  ( - b0r - i b0i )
   *  ( b3r + i b3i )     ( {a3r - a1r} + i{a3i - a1i} )     ( - b1r - i b1i ) 
   */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() = -s1.elem(0).elem(col).real();
    d.elem(2).elem(col).imag() = -s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() = -s1.elem(1).elem(col).real();
    d.elem(3).elem(col).imag() = -s1.elem(1).elem(col).imag();
  }
 

  return d;
}

template<>
inline UnaryReturn<Spin2, FnSpinReconstructDir3Plus>::Type_t
spinReconstructDir3Plus(const Spin2& s1)
{
  UnaryReturn<Spin2, FnSpinReconstructDir3Plus>::Type_t  d;

  for(int col=0; col < 3; col++) { 
    d.elem(0).elem(col).real() = s1.elem(0).elem(col).real();
    d.elem(0).elem(col).imag() = s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(1).elem(col).real() = s1.elem(1).elem(col).real();
    d.elem(1).elem(col).imag() = s1.elem(1).elem(col).imag();
  }


  /*                              ( 1  0  1  0)  ( a0 )    ( a0 + a2 )
   *  B  :=  ( 1 + Gamma  ) A  =  ( 0  1  0  1)  ( a1 )  = ( a1 + a3 )
   *                    3         ( 1  0  1  0)  ( a2 )    ( a2 + a0 )
   *                              ( 0  1  0  1)  ( a3 )    ( a3 + a1 )
   
   * The bottom components of be may be reconstructed using the formula
   
   *   ( b2r + i b2i )  =  ( {a2r + a0r} + i{a2i + a0i} )  =  ( b0r + i b0i )
   *   ( b3r + i b3i )     ( {a3r + a1r} + i{a3i + a1i} )     ( b1r + i b1i ) 
   */
  
  for(int col=0; col < 3; col++) { 
    d.elem(2).elem(col).real() =  s1.elem(0).elem(col).real();
    d.elem(2).elem(col).imag() =  s1.elem(0).elem(col).imag();
  }

  for(int col=0; col < 3; col++) { 
    d.elem(3).elem(col).real() =  s1.elem(1).elem(col).real();
    d.elem(3).elem(col).imag() =  s1.elem(1).elem(col).imag();
  }
 

  return d;
}
#endif

QDP_END_NAMESPACE();

