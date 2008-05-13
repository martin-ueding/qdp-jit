// $Id: qdp_scalarsite_sse.cc,v 1.34 2008-05-13 20:01:53 bjoo Exp $

/*! @file
 * @brief Intel SSE optimizations
 * 
 * SSE optimizations of basic operations
 */


#include "qdp.h"


// These SSE asm instructions are only supported under GCC/G++
#if defined(__GNUC__)
#include "qdp_sse_intrin.h"
namespace QDP {

#if BASE_PRECISION==32


#if 1
//-------------------------------------------------------------------
// Specialization to optimize the case   
//    LatticeColorMatrix[ Subset] = LatticeColorMatrix * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiply, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_eq_M_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right());

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 

      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, dm);
			     
    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, dm);

    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] = adj(LatticeColorMatrix) * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiply, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_eq_aM_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right());

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, dm);

    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {

      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, dm);

    }
  }

}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] = LatticeColorMatrix * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiplyAdj, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_eq_M_times_aM" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, dm);

    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *dm = (su3_matrixf *)&(d.elem(i).elem().elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, dm);

    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] = adj(LatticeColorMatrix) * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiplyAdj, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_eq_Ma_times_Ma" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);
      
      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() =  tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() = -tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() =  tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() = -tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() =  tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() = -tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() =  tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() = -tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() =  tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() = -tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() =  tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() = -tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() =  tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() = -tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() =  tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() = -tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() =  tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() = -tmp.elem(2,2).imag();
    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);
      
      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() =  tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() = -tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() =  tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() = -tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() =  tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() = -tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() =  tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() = -tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() =  tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() = -tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() =  tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() = -tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() =  tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() = -tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() =  tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() = -tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() =  tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() = -tmp.elem(2,2).imag();
    }
  }
}

//-------------------------------------------------------------------

// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] += LatticeColorMatrix * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAddAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiply, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_peq_M_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, tmpm);

      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] += adj(LatticeColorMatrix) * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAddAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiply, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_peq_aM_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] += LatticeColorMatrix * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAddAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiplyAdj, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_peq_M_times_aM" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, tmpm);

      
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] += adj(LatticeColorMatrix) * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAddAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiplyAdj, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_peq_Ma_times_Ma" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);
      
      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();

    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); ++j) {
      int i = tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);
      
      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() += tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() += tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() += tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() += tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() += tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() += tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() += tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() += tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() += tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
}

//-------------------------------------------------------------------
// Specialization to optimize the case
//   LatticeColorMatrix = LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAssign& op, 
	      const QDPExpr<
	         UnaryNode<OpIdentity, Reference< QDPType< TCol, OLattice< TCol > > > >,
                 OLattice< TCol > >& rhs, 
	      const Subset& s) 
{
  typedef OLattice<TCol> C;
  const C& l = static_cast<const C&>(rhs.expression().child());


  if( s.hasOrderedRep() ) {

    const int start = s.start();
    const int end = s.end();

    REAL* d_ptr =&(d.elem(start).elem().elem(0,0).real());
    const REAL* r_ptr =&(l.elem(start).elem().elem(0,0).real());
  
    const unsigned int total_reals = (end-start+1)*3*3*2;
    const unsigned int total_v4sf = total_reals/4;
    const unsigned int remainder = total_reals%4;
  
    float* d_ptr_v4sf = (float *)d_ptr;
    float* r_ptr_v4sf = (float *)r_ptr;

  
    for(unsigned int i = 0 ; i < total_v4sf; i++, d_ptr_v4sf +=4, r_ptr_v4sf+=4 ) { 
      _mm_store_ps( d_ptr_v4sf, _mm_load_ps(r_ptr_v4sf));
    }
  
    
    r_ptr = (REAL *)r_ptr_v4sf;
    d_ptr = (REAL *)d_ptr_v4sf;
    for(unsigned int i=0; i < remainder; i++, r_ptr++, d_ptr++) { 
      *d_ptr = *r_ptr;
    }
  }
  else {
    // Unordered case 
    const int* tab = s.siteTable().slice();
    
    // Loop through the sites
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i = tab[j];

      // Do the copy in the dumb way -- this could become quite complex
      // Depending on whether the individual matrices are aligned or not.
      d.elem(i).elem() = l.elem(i).elem();

    }

  }
}

//-------------------------------------------------------------------
// Specialization to optimize the case
//   LatticeColorMatrix += LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpAddAssign& op, 
	      const QDPExpr<
	         UnaryNode<OpIdentity, Reference< QDPType< TCol, OLattice< TCol > > > >,
                 OLattice< TCol > >& rhs, 
	      const Subset& s) 
{
  typedef OLattice<TCol> C;
  const C& l = static_cast<const C&>(rhs.expression().child());

  if( s.hasOrderedRep() ) { 
    const int start = s.start();
    const int end = s.end();

    REAL* d_ptr =&(d.elem(start).elem().elem(0,0).real());
    const REAL* r_ptr =&(l.elem(start).elem().elem(0,0).real());
    
    const unsigned int total_reals = (end-start+1)*3*3*2;
    const unsigned int total_v4sf = total_reals/4;
    const unsigned int remainder = total_reals%4;
    
    float* d_ptr_v4sf = (float *)d_ptr;
    float* r_ptr_v4sf = (float *)r_ptr;
    
    
    for(unsigned int i = 0 ; i < total_v4sf; i++, d_ptr_v4sf +=4, r_ptr_v4sf+=4 ) { 
      _mm_store_ps( d_ptr_v4sf, _mm_add_ps( _mm_load_ps(d_ptr_v4sf), 
					    _mm_load_ps(r_ptr_v4sf) ) );
    }
    
    
    r_ptr = (REAL *)r_ptr_v4sf;
    d_ptr = (REAL *)d_ptr_v4sf;
    for(unsigned int i=0; i < remainder; i++, r_ptr++, d_ptr++) { 
      *d_ptr += *r_ptr;
    }
  }
  else {
    // Unordered case 
    const int* tab = s.siteTable().slice();
    
    // Loop through the sites
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i = tab[j];

      // Do the copy in the dumb way -- this could become quite complex
      // Depending on whether the individual matrices are aligned or not.
      d.elem(i).elem() += l.elem(i).elem();

    }

  }
}

//-------------------------------------------------------------------
// Specialization to optimize the case
//   LatticeColorMatrix -= LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpSubtractAssign& op, 
	      const QDPExpr<
	         UnaryNode<OpIdentity, Reference< QDPType< TCol, OLattice< TCol > > > >,
                 OLattice< TCol > >& rhs, 
	      const Subset& s) 
{
  typedef OLattice<TCol> C;
  const C& l = static_cast<const C&>(rhs.expression().child());
  if (s.hasOrderedRep()) { 
    const int start = s.start();
    const int end = s.end();
    
    REAL* d_ptr =&(d.elem(start).elem().elem(0,0).real());
    const REAL* r_ptr =&(l.elem(start).elem().elem(0,0).real());
    
    const unsigned int total_reals = (end-start+1)*3*3*2;
    const unsigned int total_v4sf = total_reals/4;
    const unsigned int remainder = total_reals%4;
    
    float* d_ptr_v4sf = (float *)d_ptr;
    float* r_ptr_v4sf = (float *)r_ptr;
    
    
    for(unsigned int i = 0 ; i < total_v4sf; i++, d_ptr_v4sf +=4, r_ptr_v4sf+=4 ) { 
      _mm_store_ps( d_ptr_v4sf, _mm_sub_ps( _mm_load_ps( d_ptr_v4sf), 
					    _mm_load_ps( r_ptr_v4sf) ) );
    }
    
    
    r_ptr = (REAL *)r_ptr_v4sf;
    d_ptr = (REAL *)d_ptr_v4sf;
    for(unsigned int i=0; i < remainder; i++, r_ptr++, d_ptr++) { 
      *d_ptr -= *r_ptr;
    }
  }
  else {   
    // Unordered case 
    const int* tab = s.siteTable().slice();
    
    // Loop through the sites
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i = tab[j];

      // Do the copy in the dumb way -- this could become quite complex
      // Depending on whether the individual matrices are aligned or not.
      d.elem(i).elem() -= l.elem(i).elem();

    }

  }
}

// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] -= LatticeColorMatrix * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpSubtractAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiply, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_meq_M_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right());

  PColorMatrix<RComplexFloat,3> tmp;


  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i=tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] -= adj(LatticeColorMatrix) * LatticeColorMatrix
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpSubtractAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiply, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >, 
	                    Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_meq_aM_times_M" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right());

  PColorMatrix<RComplexFloat,3> tmp;
  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, tmpm);

      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i=tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_an(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] -= LatticeColorMatrix * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpSubtractAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiplyAdj, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_meq_M_times_aM" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  PColorMatrix<RComplexFloat,3> tmp;

  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i=tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_na(lm, rm, tmpm);
      
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() -= tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(0,1).imag() -= tmp.elem(0,1).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(0,2).imag() -= tmp.elem(0,2).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(1,0).imag() -= tmp.elem(1,0).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() -= tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(1,2).imag() -= tmp.elem(1,2).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(2,0).imag() -= tmp.elem(2,0).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(2,1).imag() -= tmp.elem(2,1).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() -= tmp.elem(2,2).imag();
    }
  }
}


// Specialization to optimize the case   
//    LatticeColorMatrix[Subset] -= adj(LatticeColorMatrix) * adj(LatticeColorMatrix)
template<>
void evaluate(OLattice< TCol >& d, 
	      const OpSubtractAssign& op, 
	      const QDPExpr<BinaryNode<OpAdjMultiplyAdj, 
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > >,
	                    UnaryNode<OpIdentity, Reference<QDPType< TCol, OLattice< TCol > > > > >,
	                    OLattice< TCol > >& rhs,
	      const Subset& s)
{
//  cout << "call single site QDP_M_meq_Ma_times_Ma" << endl;

  typedef OLattice< TCol >    C;

  const C& l = static_cast<const C&>(rhs.expression().left().child());
  const C& r = static_cast<const C&>(rhs.expression().right().child());

  PColorMatrix<RComplexFloat,3> tmp;
  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);

      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();

    }
  }
  else { 
    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i=tab[j];
      su3_matrixf *lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      su3_matrixf *rm = (su3_matrixf *)&(r.elem(i).elem().elem(0,0).real());
      su3_matrixf *tmpm = (su3_matrixf *)&(tmp.elem(0,0).real());

      intrin_sse_mult_su3_nn(rm, lm, tmpm);
      
      
      // Take the adj(r*l) = adj(l)*adj(r)
      d.elem(i).elem().elem(0,0).real() -= tmp.elem(0,0).real();
      d.elem(i).elem().elem(0,0).imag() += tmp.elem(0,0).imag();
      d.elem(i).elem().elem(0,1).real() -= tmp.elem(1,0).real();
      d.elem(i).elem().elem(0,1).imag() += tmp.elem(1,0).imag();
      d.elem(i).elem().elem(0,2).real() -= tmp.elem(2,0).real();
      d.elem(i).elem().elem(0,2).imag() += tmp.elem(2,0).imag();
      
      d.elem(i).elem().elem(1,0).real() -= tmp.elem(0,1).real();
      d.elem(i).elem().elem(1,0).imag() += tmp.elem(0,1).imag();
      d.elem(i).elem().elem(1,1).real() -= tmp.elem(1,1).real();
      d.elem(i).elem().elem(1,1).imag() += tmp.elem(1,1).imag();
      d.elem(i).elem().elem(1,2).real() -= tmp.elem(2,1).real();
      d.elem(i).elem().elem(1,2).imag() += tmp.elem(2,1).imag();
      
      d.elem(i).elem().elem(2,0).real() -= tmp.elem(0,2).real();
      d.elem(i).elem().elem(2,0).imag() += tmp.elem(0,2).imag();
      d.elem(i).elem().elem(2,1).real() -= tmp.elem(1,2).real();
      d.elem(i).elem().elem(2,1).imag() += tmp.elem(1,2).imag();
      d.elem(i).elem().elem(2,2).real() -= tmp.elem(2,2).real();
      d.elem(i).elem().elem(2,2).imag() += tmp.elem(2,2).imag();
    }
  }
}


//-------------------------------------------------------------------

// Specialization to optimize the case   
//    LatticeHalfFermion = LatticeColorMatrix * LatticeHalfFermion
// NOTE: let this be a subroutine to save space
template<>
void evaluate(OLattice< TVec2 >& d, 
	      const OpAssign& op, 
	      const QDPExpr<BinaryNode<OpMultiply, 
	                    Reference<QDPType< TCol, OLattice< TCol > > >, 
	                    Reference<QDPType< TVec2, OLattice< TVec2 > > > >,
	                    OLattice< TVec2 > >& rhs,
	      const Subset& s)
{
#if defined(QDP_SCALARSITE_DEBUG)
  cout << "specialized QDP_H_M_times_H" << endl;
#endif

  typedef OLattice<PScalar<PColorMatrix<RComplexFloat, 3> > >       C;
  typedef OLattice<PSpinVector<PColorVector<RComplexFloat, 3>, 2> > H;

  const C& l = static_cast<const C&>(rhs.expression().left());
  const H& r = static_cast<const H&>(rhs.expression().right());



  if( s.hasOrderedRep() ) { 
    for(int i=s.start(); i <= s.end(); i++) { 

      su3_matrixf* lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      half_wilson_vectorf* rh = (half_wilson_vectorf*)&(r.elem(i).elem(0).elem(0).real());
      half_wilson_vectorf* dh = (half_wilson_vectorf*)&(d.elem(i).elem(0).elem(0).real());

      intrin_sse_mult_su3_mat_hwvec(lm,rh,dh);

    }
  }
  else { 

    const int *tab = s.siteTable().slice();
    for(int j=0; j < s.numSiteTable(); j++) { 
      int i=tab[j];
      su3_matrixf* lm = (su3_matrixf *)&(l.elem(i).elem().elem(0,0).real());
      half_wilson_vectorf* rh = (half_wilson_vectorf*)&(r.elem(i).elem(0).elem(0).real());
      half_wilson_vectorf* dh = (half_wilson_vectorf*)&(d.elem(i).elem(0).elem(0).real());

      intrin_sse_mult_su3_mat_hwvec(lm,rh,dh);
    }
  }
}
#endif


//-------------------------------------------------------------------
// GNUC vector type

//#define DEBUG_BLAS

// AXPY and AXMY routines
void vaxpy3(REAL32 *Out,REAL32 *scalep,REAL32 *InScale, REAL32 *Add,int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vaxpy3" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

  v4sf vscalep = _mm_load_ss(scalep);
  vscalep = _mm_shuffle_ps(vscalep, vscalep, 0);


  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 0)), _mm_load_ps(Add+ 0)));
    _mm_store_ps(Out+ 4, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 4)), _mm_load_ps(Add+ 4)));
    _mm_store_ps(Out+ 8, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 8)), _mm_load_ps(Add+ 8)));
    _mm_store_ps(Out+12, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+12)), _mm_load_ps(Add+12)));
    _mm_store_ps(Out+16, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+16)), _mm_load_ps(Add+16)));
    _mm_store_ps(Out+20, _mm_add_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+20)), _mm_load_ps(Add+20)));

    Out += 24; InScale += 24; Add += 24;
  }
}


void vaxmy3(REAL32 *Out,REAL32 *scalep,REAL32 *InScale, REAL32 *Sub,int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vaxmy3" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

//  register v4sf va = load_v4sf((float *)&a);
  v4sf vscalep = _mm_load_ss(scalep);
  vscalep = _mm_shuffle_ps( vscalep, vscalep, 0);

  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 0)), _mm_load_ps(Sub+ 0)));
    _mm_store_ps(Out+ 4, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 4)), _mm_load_ps(Sub+ 4)));
    _mm_store_ps(Out+ 8, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+ 8)), _mm_load_ps(Sub+ 8)));
    _mm_store_ps(Out+12, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+12)), _mm_load_ps(Sub+12)));
    _mm_store_ps(Out+16, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+16)), _mm_load_ps(Sub+16)));
    _mm_store_ps(Out+20, _mm_sub_ps(_mm_mul_ps(vscalep, _mm_load_ps(InScale+20)), _mm_load_ps(Sub+20)));

    Out += 24; InScale += 24; Sub += 24;
  }
}


void vadd(REAL32 *Out, REAL32 *In1, REAL32 *In2, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vadd" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_add_ps(_mm_load_ps(In1+ 0), _mm_load_ps(In2+ 0)));
    _mm_store_ps(Out+ 4, _mm_add_ps(_mm_load_ps(In1+ 4), _mm_load_ps(In2+ 4)));
    _mm_store_ps(Out+ 8, _mm_add_ps(_mm_load_ps(In1+ 8), _mm_load_ps(In2+ 8)));
    _mm_store_ps(Out+12, _mm_add_ps(_mm_load_ps(In1+12), _mm_load_ps(In2+12)));
    _mm_store_ps(Out+16, _mm_add_ps(_mm_load_ps(In1+16), _mm_load_ps(In2+16)));
    _mm_store_ps(Out+20, _mm_add_ps(_mm_load_ps(In1+20), _mm_load_ps(In2+20)));

    Out += 24; In1 += 24; In2 += 24;
  }
}


void vsub(REAL32 *Out, REAL32 *In1, REAL32 *In2, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vsub" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_sub_ps(_mm_load_ps(In1+ 0), _mm_load_ps(In2+ 0)));
    _mm_store_ps(Out+ 4, _mm_sub_ps(_mm_load_ps(In1+ 4), _mm_load_ps(In2+ 4)));
    _mm_store_ps(Out+ 8, _mm_sub_ps(_mm_load_ps(In1+ 8), _mm_load_ps(In2+ 8)));
    _mm_store_ps(Out+12, _mm_sub_ps(_mm_load_ps(In1+12), _mm_load_ps(In2+12)));
    _mm_store_ps(Out+16, _mm_sub_ps(_mm_load_ps(In1+16), _mm_load_ps(In2+16)));
    _mm_store_ps(Out+20, _mm_sub_ps(_mm_load_ps(In1+20), _mm_load_ps(In2+20)));

    Out += 24; In1 += 24; In2 += 24;
  }
}

void vscal(REAL32 *Out, REAL32 *scalep, REAL32 *In, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vadd" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

//  register v4sf va = load_v4sf((float *)&a);
  v4sf vscalep = _mm_load_ss(scalep);
  vscalep = _mm_shuffle_ps(vscalep, vscalep, 0);

  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_mul_ps(vscalep, _mm_load_ps(In+ 0)));
    _mm_store_ps(Out+ 4, _mm_mul_ps(vscalep, _mm_load_ps(In+ 4)));
    _mm_store_ps(Out+ 8, _mm_mul_ps(vscalep, _mm_load_ps(In+ 8)));
    _mm_store_ps(Out+12, _mm_mul_ps(vscalep, _mm_load_ps(In+12)));
    _mm_store_ps(Out+16, _mm_mul_ps(vscalep, _mm_load_ps(In+16)));
    _mm_store_ps(Out+20, _mm_mul_ps(vscalep, _mm_load_ps(In+20)));

    Out += 24; In += 24;
  }
}  


void vaxpby3(REAL32 *Out, REAL32 *a, REAL32 *x, REAL32 *b, REAL32 *y, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vaxpby3: a*x+b*y" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

//  register v4sf va = load_v4sf((float *)&a);
  v4sf va = _mm_load_ss(a);
  v4sf vb = _mm_load_ss(b);
  va = _mm_shuffle_ps(va, va, 0);
  vb = _mm_shuffle_ps(vb, vb, 0);


  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+ 0)), _mm_mul_ps(vb, _mm_load_ps(y+ 0))));
    _mm_store_ps(Out+ 4, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+ 4)), _mm_mul_ps(vb, _mm_load_ps(y+ 4))));
    _mm_store_ps(Out+ 8, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+ 8)), _mm_mul_ps(vb, _mm_load_ps(y+ 8))));
    _mm_store_ps(Out+12, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+12)), _mm_mul_ps(vb, _mm_load_ps(y+12))));
    _mm_store_ps(Out+16, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+16)), _mm_mul_ps(vb, _mm_load_ps(y+16))));
    _mm_store_ps(Out+20, _mm_add_ps(_mm_mul_ps(va, _mm_load_ps(x+20)), _mm_mul_ps(vb, _mm_load_ps(y+20))));

    Out += 24; x += 24; y += 24;
  }
}


void vaxmby3(REAL32 *Out, REAL32 *a, REAL32 *x, REAL32 *b, REAL32 *y, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: vaxmby3: a*x-b*y" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors

//  register v4sf va = load_v4sf((float *)&a);
  v4sf va = _mm_load_ss(a);
  v4sf vb = _mm_load_ss(b);
  va = _mm_shuffle_ps( va, va, 0);
  vb = _mm_shuffle_ps( vb, vb, 0);

  for (; n_loops-- > 0; )
  {
    _mm_store_ps(Out+ 0, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+ 0)), _mm_mul_ps(vb, _mm_load_ps(y+ 0))));
    _mm_store_ps(Out+ 4, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+ 4)), _mm_mul_ps(vb, _mm_load_ps(y+ 4))));
    _mm_store_ps(Out+ 8, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+ 8)), _mm_mul_ps(vb, _mm_load_ps(y+ 8))));
    _mm_store_ps(Out+12, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+12)), _mm_mul_ps(vb, _mm_load_ps(y+12))));
    _mm_store_ps(Out+16, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+16)), _mm_mul_ps(vb, _mm_load_ps(y+16))));
    _mm_store_ps(Out+20, _mm_sub_ps(_mm_mul_ps(va, _mm_load_ps(x+20)), _mm_mul_ps(vb, _mm_load_ps(y+20))));

    Out += 24; x += 24; y += 24;
  }
}



void local_sumsq(REAL64 *Out, REAL32 *In, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: local_sumsq" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors


  (*Out) = (REAL64)0;

  register SSEVec in0;
  register SSEVec in1;
  register SSEVec in2;
  register SSEVec in3;
  register SSEVec sq;
  register SSEVec sum;

  n_loops--;  // We pull out the first 4

  sum.floats[0] = sum.floats[1] = sum.floats[2] = sum.floats[3] = 0;

  in0.vector = _mm_load_ps(In);
   sq.vector = _mm_mul_ps(in0.vector,in0.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);
  
  in1.vector = _mm_load_ps(In+4);
  sq.vector = _mm_mul_ps(in1.vector,in1.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);
  
  in2.vector = _mm_load_ps(In+8);
  sq.vector = _mm_mul_ps(in2.vector,in2.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);

  in3.vector = _mm_load_ps(In+12);
  sq.vector = _mm_mul_ps(in3.vector,in3.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);

  *Out += (double)sum.floats[0] 
    + (double)sum.floats[1] 
    + (double)sum.floats[2]
    + (double)sum.floats[3];
  
  In += 16;

  for (; n_loops-- > 0; ) {
    
    // Initialise the sum
    sum.floats[0] = sum.floats[1] = sum.floats[2] = sum.floats[3] = 0;

    // Do 24
    
    in0.vector = _mm_load_ps(In);
    sq.vector = _mm_mul_ps(in0.vector,in0.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);

    in1.vector = _mm_load_ps(In+4);
    sq.vector = _mm_mul_ps(in1.vector,in1.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
      
    in2.vector = _mm_load_ps(In+8);
    sq.vector = _mm_mul_ps(in2.vector,in2.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in3.vector = _mm_load_ps(In+12);
    sq.vector = _mm_mul_ps(in3.vector,in3.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in0.vector = _mm_load_ps(In+16);
    sq.vector = _mm_mul_ps(in0.vector,in0.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in1.vector = _mm_load_ps(In+20);
    sq.vector = _mm_mul_ps(in1.vector,in1.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);

  *Out += (double)sum.floats[0] 
    + (double)sum.floats[1] 
    + (double)sum.floats[2]
    + (double)sum.floats[3];

    In +=24;

  }

  sum.floats[0] = sum.floats[1] = sum.floats[2] = sum.floats[3] = 0;

  in2.vector = _mm_load_ps(In);
  sq.vector = _mm_mul_ps(in2.vector,in2.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);

  in3.vector = _mm_load_ps(In+4);
  sq.vector = _mm_mul_ps(in3.vector,in3.vector);
  sum.vector = _mm_add_ps(sum.vector, sq.vector);

  *Out += (double)sum.floats[0] 
    + (double)sum.floats[1] 
    + (double)sum.floats[2]
    + (double)sum.floats[3];

}

void local_sumsq2(REAL64 *Out, REAL32 *In, int n_3vec)
{
#ifdef DEBUG_BLAS
  QDPIO::cout << "SSE_TEST: local_sumsq" << endl;
#endif

//  int n_loops = n_3vec >> 2;   // only works on multiple of length 4 vectors
  int n_loops = n_3vec / 24;   // only works on multiple of length 24 vectors


  (*Out) = (REAL64)0;

  register SSEVec in;
  register SSEVec sq;
  register SSEVec sum;


  sum.floats[0] = sum.floats[1] = sum.floats[2] = sum.floats[3] = 0;
  for (; n_loops-- > 0; ) {
    

    // Do 24
    
    in.vector = _mm_load_ps(In);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);

    in.vector = _mm_load_ps(In+4);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
      
    in.vector = _mm_load_ps(In+8);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in.vector = _mm_load_ps(In+12);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in.vector = _mm_load_ps(In+16);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);
    
    in.vector = _mm_load_ps(In+20);
    sq.vector = _mm_mul_ps(in.vector,in.vector);
    sum.vector = _mm_add_ps(sum.vector, sq.vector);

    In +=24;

  }

  *Out += (double)sum.floats[0] 
    + (double)sum.floats[1] 
    + (double)sum.floats[2]
    + (double)sum.floats[3];

}


void local_vcdot(REAL64 *Out_re, REAL64 *Out_im, REAL32 *V1, REAL32 *V2, int n_3vec)
{
  register double result_re;
  register double result_im;

  
  register double v1_0r;
  register double v1_0i;
  register double v1_1r;
  register double v1_1i;
  register double v1_2r;
  register double v1_2i;

  register double v2_0r;
  register double v2_0i;
  register double v2_1r;
  register double v2_1i;
  register double v2_2r;
  register double v2_2i;

  register int counter=0;
  register unsigned long vecptr1=0;
  register unsigned long vecptr2=0;
  result_re= 0;
  result_im= 0;

  
  if( n_3vec > 0 ) { 

    v1_0r = (REAL64)V1[vecptr1++];
    v2_0r = (REAL64)V2[vecptr2++];
    
    v1_0i = (REAL64)V1[vecptr1++];
    v2_0i = (REAL64)V2[vecptr2++];
    
    v1_1r = (REAL64)V1[vecptr1++];
    v2_1r = (REAL64)V2[vecptr2++];

    for(counter=0; counter < n_3vec-1; counter++) {


      
      result_re = result_re + v1_0r*v2_0r;
      v1_1i =(REAL64)V1[vecptr1++];
      result_im = result_im - v1_0i*v2_0r;
      v2_1i = (REAL64)V2[vecptr2++];
      result_im = result_im + v1_0r*v2_0i;
      v1_2r = (REAL64)V1[vecptr1++];
      result_re = result_re + v1_0i*v2_0i;
      v2_2r = (REAL64)V2[vecptr2++];
      
      result_re = result_re + v1_1r*v2_1r;
      v1_2i = (REAL64)V1[vecptr1++];
      result_im = result_im - v1_1i*v2_1r;
      v2_2i = (REAL64)V2[vecptr2++];
      result_im = result_im + v1_1r*v2_1i;
      v1_0r = (REAL64)V1[vecptr1++];
      result_re = result_re + v1_1i*v2_1i;
      v2_0r = (REAL64)V2[vecptr2++];

      result_re = result_re + v1_2r*v2_2r;
      v1_0i = (REAL64)V1[vecptr1++];
      result_im = result_im - v1_2i*v2_2r;
      v2_0i = (REAL64)V2[vecptr2++];
      result_im = result_im + v1_2r*v2_2i;
      v1_1r = (REAL64)V1[vecptr1++];
      result_re = result_re + v1_2i*v2_2i;
      v2_1r = (REAL64)V2[vecptr2++];

    }

    // Last one plus drain...
    result_re = result_re + v1_0r*v2_0r;
    v1_1i =(REAL64)V1[vecptr1++];
    result_im = result_im - v1_0i*v2_0r;
    v2_1i = (REAL64)V2[vecptr2++];
    result_im = result_im + v1_0r*v2_0i;
    v1_2r = (REAL64)V1[vecptr1++];
    result_re = result_re + v1_0i*v2_0i;
    v2_2r = (REAL64)V2[vecptr2++];
      
    result_re = result_re + v1_1r*v2_1r;
    v1_2i = (REAL64)V1[vecptr1++];
    result_im = result_im - v1_1i*v2_1r;
    v2_2i = (REAL64)V2[vecptr2++];
 
    result_im = result_im + v1_1r*v2_1i;
    result_re = result_re + v1_1i*v2_1i;
    

    result_re = result_re + v1_2r*v2_2r;
    result_im = result_im - v1_2i*v2_2r;
    result_im = result_im + v1_2r*v2_2i;
    result_re = result_re + v1_2i*v2_2i;    

  }
  
  *Out_re=(REAL64)result_re;
  *Out_im=(REAL64)result_im;
}


void local_vcdot_real(REAL64 *Out, REAL32 *V1, REAL32 *V2, int n_3vec)
{
  register REAL64 result;
  
  register REAL64 v1_0r;
  register REAL64 v1_0i;
  register REAL64 v1_1r;
  register REAL64 v1_1i;
  register REAL64 v1_2r;
  register REAL64 v1_2i;

  register REAL64 v2_0r;
  register REAL64 v2_0i;
  register REAL64 v2_1r;
  register REAL64 v2_1i;
  register REAL64 v2_2r;
  register REAL64 v2_2i;

  register int counter=0;
  register unsigned long vecptr1=0;
  register unsigned long vecptr2=0;
  result= 0;

  
  if( n_3vec > 0 ) { 

    // Prefetch 
    v1_0r = (REAL64)V1[vecptr1++];
    v2_0r = (REAL64)V2[vecptr2++];

    v1_0i = (REAL64)V1[vecptr1++];
    v2_0i = (REAL64)V2[vecptr2++];

    v1_1r = (REAL64)V1[vecptr1++];
    v2_1r = (REAL64)V2[vecptr2++];

    v1_1i =(REAL64)V1[vecptr1++];
    v2_1i = (REAL64)V2[vecptr2++];
    
    v1_2r = (REAL64)V1[vecptr1++];
    v2_2r = (REAL64)V2[vecptr2++];
    
    v1_2i = (REAL64)V1[vecptr1++];
    v2_2i = (REAL64)V2[vecptr2++];

    for(counter=0; counter < n_3vec-1; counter++) {
      result = result + v1_0r*v2_0r;
      v1_0r = (REAL64)V1[vecptr1++];
      v2_0r = (REAL64)V2[vecptr2++];    

      result = result + v1_0i*v2_0i;
      v1_0i = (REAL64)V1[vecptr1++];
      v2_0i = (REAL64)V2[vecptr2++];
      
      result = result + v1_1r*v2_1r;
      v1_1r = (REAL64)V1[vecptr1++];
      v2_1r = (REAL64)V2[vecptr2++];

      result = result + v1_1i*v2_1i;
      v1_1i =(REAL64)V1[vecptr1++];
      v2_1i = (REAL64)V2[vecptr2++];
      
      result = result + v1_2r*v2_2r;
      v1_2r = (REAL64)V1[vecptr1++];
      v2_2r = (REAL64)V2[vecptr2++];

      result = result + v1_2i*v2_2i;
      v1_2i = (REAL64)V1[vecptr1++];
      v2_2i = (REAL64)V2[vecptr2++];


    }

    // Last one plus drain...
    result = result + v1_0r*v2_0r;
    result = result + v1_0i*v2_0i;
    result = result + v1_1r*v2_1r;
    result = result + v1_1i*v2_1i;
    result = result + v1_2r*v2_2r;
    result = result + v1_2i*v2_2i;    

  }
  
  *Out=(REAL64)result;
}



#endif // BASE PRECISION==32

} // namespace QDP;

#endif  // defined(__GNUC__)
