#include "qdp.h"

#include <array>

namespace QDP {


  // These functions convert a linear space-time index into a index vector
  // as used for the coalesced memory accesses.


  namespace LLVMOPT {
    int64_t BBsearch = 400;
  }


  int64_t getBBSearchLimit() {
    return LLVMOPT::BBsearch;
  }

  void setBBSearchLimit( int64_t i ) {
    LLVMOPT::BBsearch = i;
  }


  IndexDomainVector get_index_vector_from_index( llvm::Value *index )
  {
    //std::cout << "Using inner length = " << DataLayout::inner << "\n";

    llvm::Value * inner = llvm_create_value( 1 );
    llvm::Value * iv_div_inner = llvm_div( index , inner );   // outer
    llvm::Value * iv_mod_inner = llvm_rem( index , inner );   // inner

    IndexDomainVector args;
    args.push_back( make_pair( Layout::sitesOnNode() , iv_div_inner ) );
    args.push_back( make_pair( 1 , iv_mod_inner ) );

    return args;
  }


  IndexDomainVector get_scalar_index_vector_from_index( llvm::Value *index )
  {
    //std::cout << "Using inner length = " << DataLayout::inner << "\n";

    IndexDomainVector args;
    args.push_back( make_pair( Layout::sitesOnNode() , index ) );
    args.push_back( make_pair( 1 , llvm_create_value(0) ) );

    return args;
  }


  llvm::Value *get_index_from_index_vector( const IndexDomainVector& idx ) {
    assert( idx.size() >= 2 );

    const size_t nIvo = 0; // volume outer
    const size_t nIvi = 1; // volume inner

    int         Lvo,Lvi;
    llvm::Value *ivo,*ivi;

    Lvo = idx.at(nIvo).first;
    ivo = idx.at(nIvo).second;

    Lvi = idx.at(nIvi).first;
    ivi = idx.at(nIvi).second;

    llvm::Value * Ivo = llvm_create_value(Lvo);
    llvm::Value * Ivi = llvm_create_value(Lvi);

    llvm::Value * iv = llvm_add(llvm_mul( ivo , Ivi ) , ivi ); // reconstruct volume index

    return iv;
  }


  std::array<int,5> QDP_jit_layout;

  void QDP_set_jit_datalayout(int pos_o, int pos_s, int pos_c, int pos_r, int pos_i) {
    QDP_jit_layout[pos_o] = 0;
    QDP_jit_layout[pos_s] = 1;
    QDP_jit_layout[pos_c] = 2;
    QDP_jit_layout[pos_r] = 3;
    QDP_jit_layout[pos_i] = 4;
  }

  void QDP_print_jit_datalayout() {
    const char* letters="oscri";
    QDPIO::cerr << "Using JIT data layout ";
    QDPIO::cerr << letters[ QDP_jit_layout[0] ];
    QDPIO::cerr << letters[ QDP_jit_layout[1] ];
    QDPIO::cerr << letters[ QDP_jit_layout[2] ];
    QDPIO::cerr << letters[ QDP_jit_layout[3] ];
    QDPIO::cerr << letters[ QDP_jit_layout[4] ];
    QDPIO::cerr << "\n";
  }


#if 1
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    // for(int i=0 ; i<a.size() ; ++i) 
    //   QDPIO::cout << i << "  " << a.at(i).first << "\n";
    assert( a.size() == 7 && "index domain vector size is not 8");

    llvm::Value* offset = llvm_create_value(0);

    // for( int i = 0 ; i < Nd ; ++i ) {
    //   llvm::Value* l_size = llvm_create_value( Layout::subgridLattSize()[a.at(i).first] );
    //   llvm::Value* l_val  = a.at(i).second;

    //   offset = llvm_add( llvm_mul( offset , l_size ) , l_val );
    // }

    //return offset;

    const size_t nIx  = 0; // x
    const size_t nIy  = 1; // y
    const size_t nIz  = 2; // z
    const size_t nIt  = 3; // t
    const size_t nIs  = 4; // spin
    const size_t nIc  = 5; // color
    const size_t nIr  = 6; // reality

    int          Lx, Ly, Lz, Lt, Ls, Lc, Lr;
    llvm::Value *ix,*iy,*iz,*it,*is,*ic,*ir;

    //Lx = a.at(nIx).first;
    ix = a.at(nIx).second;

    //Ly = a.at(nIy).first;
    iy = a.at(nIy).second;

    //Lz = a.at(nIz).first;
    iz = a.at(nIz).second;

    //Lt = a.at(nIt).first;
    it = a.at(nIt).second;

    Ls = a.at(nIs).first;
    is = a.at(nIs).second;

    Lc = a.at(nIc).first;
    ic = a.at(nIc).second;

    Lr = a.at(nIr).first;
    ir = a.at(nIr).second;
    
    llvm::Value * Ix = llvm_create_value( Layout::subgridLattSize()[0] );
    llvm::Value * Iy = llvm_create_value( Layout::subgridLattSize()[1] );
    llvm::Value * Iz = llvm_create_value( Layout::subgridLattSize()[2] );
    llvm::Value * It = llvm_create_value( Layout::subgridLattSize()[3] );
    llvm::Value * Is = llvm_create_value(Ls);
    llvm::Value * Ic = llvm_create_value(Lc);
    llvm::Value * Ir = llvm_create_value(Lr);

    //QDPIO::cout << Is << " " << Ic << " " << Ir << "\n";

      // llvm::Value* l_size = llvm_create_value( Layout::subgridLattSize()[a.at(i).first] );
      // llvm::Value* l_val  = a.at(i).second;
      // offset = llvm_add( llvm_mul( offset , l_size ) , l_val );

    //llvm::Value* offset = llvm_create_value(0);

    llvm::Value * ix_m4 = llvm_rem( ix , llvm_create_value((int)4) );
    llvm::Value * ix_d4 = llvm_div( ix , llvm_create_value((int)4) );

    offset = llvm_add( llvm_mul( offset , It ) , it );
    offset = llvm_add( llvm_mul( offset , Iz ) , iz );
    offset = llvm_add( llvm_mul( offset , Iy ) , iy );
    offset = llvm_add( llvm_mul( offset , llvm_div(Ix,llvm_create_value((int)4) ) ) , ix_d4 );
    offset = llvm_add( llvm_mul( offset , Is ) , is );
    offset = llvm_add( llvm_mul( offset , Ic ) , ic );
    offset = llvm_add( llvm_mul( offset , Ir ) , ir );
    offset = llvm_add( llvm_mul( offset , llvm_create_value((int)4) ) , ix_m4 );

    return offset;

  }
#endif


#if 0
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    if ( a.size() == 5 ) {
      const size_t nIvo = 0; // volume outer
      const size_t nIvi = 1; // volume inner
      const size_t nIs  = 2; // spin
      const size_t nIc  = 3; // color
      const size_t nIr  = 4; // reality

      int         Lvo,Lvi,Ls,Lc,Lr;
      llvm::Value *ivo,*ivi,*is,*ic,*ir;

      Lvo = a.at(nIvo).first;
      ivo = a.at(nIvo).second;

      Lvi = a.at(nIvi).first;
      ivi = a.at(nIvi).second;

      Ls = a.at(nIs).first;
      is = a.at(nIs).second;

      Lc = a.at(nIc).first;
      ic = a.at(nIc).second;

      Lr = a.at(nIr).first;
      ir = a.at(nIr).second;

      llvm::Value * Ivo = llvm_create_value(Lvo);
      llvm::Value * Ivi = llvm_create_value(Lvi);
      llvm::Value * Is = llvm_create_value(Ls);
      llvm::Value * Ic = llvm_create_value(Lc);
      llvm::Value * Ir = llvm_create_value(Lr);

      // llvm::Value * iv_div_inner = llvm_div( iv , inner ); // outer
      // llvm::Value * iv_mod_inner = llvm_rem( iv , inner ); // inner


      // offset = ((ir * Ic + ic) * Is + is) * Iv + iv

      std::array<llvm::Value *,5> dom_in;
      dom_in[0] = Ivo;
      dom_in[1] = Is;
      dom_in[2] = Ic;
      dom_in[3] = Ir;
      dom_in[4] = Ivi;
      std::array<llvm::Value *,5> ind_in;
      ind_in[0] = ivo;
      ind_in[1] = is;
      ind_in[2] = ic;
      ind_in[3] = ir;
      ind_in[4] = ivi;

      std::array<llvm::Value *,5> dom;
      dom[0] = dom_in[ QDP_jit_layout[0] ];
      dom[1] = dom_in[ QDP_jit_layout[1] ];
      dom[2] = dom_in[ QDP_jit_layout[2] ];
      dom[3] = dom_in[ QDP_jit_layout[3] ];
      dom[4] = dom_in[ QDP_jit_layout[4] ];
      std::array<llvm::Value *,5> ind;
      ind[0] = ind_in[ QDP_jit_layout[0] ];
      ind[1] = ind_in[ QDP_jit_layout[1] ];
      ind[2] = ind_in[ QDP_jit_layout[2] ];
      ind[3] = ind_in[ QDP_jit_layout[3] ];
      ind[4] = ind_in[ QDP_jit_layout[4] ];
      

      if (lay == JitDeviceLayout::LayoutCoalesced) {
	return llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(ind[0],dom[1]),ind[1]),dom[2]),ind[2]),dom[3]),ind[3]),dom[4]),ind[4]);
	//return llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(ivo,Is),is),Ic),ic),Ir),ir),Ivi),ivi); // orig
	//return llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(ivo,Ic),ic),Is),is),Ir),ir),Ivi),ivi); // ok
	//return llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(ivo,Is),is),Ic),ic),Ivi),ivi),Ir),ir); // ok
      } else {
	llvm::Value * iv = llvm_add(llvm_mul( ivo , Ivi ) , ivi ); // reconstruct volume index
	return llvm_add(llvm_mul(llvm_add(llvm_mul(llvm_add(llvm_mul(iv,Is),is),Ic),ic),Ir),ir);
      }
    } else {
      // We support non-full DomainIndexVectors
      // This is needed e.g. for peek instructions
      assert( lay == JitDeviceLayout::LayoutScalar );
      llvm::Value * offset = llvm_create_value(0);
      for( IndexDomainVector::const_iterator x = a.begin() ; x != a.end() ; x++ ) {
	int         Index;
	llvm::Value * index;
	Index = x->first;
	index = x->second;
	llvm::Value * Index_jit = llvm_create_value(Index);
	offset = llvm_add( llvm_mul( offset , Index_jit ) , index );
      }
      return offset;
    }
  }
#endif


#if 0
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    llvm::Value * inner = llvm_create_value( 4 );

    const size_t nIv = 0; // volume
    const size_t nIs = 1; // spin
    const size_t nIc = 2; // color
    const size_t nIr = 3; // reality

    int         Lv,Ls,Lc,Lr;
    llvm::Value *iv,*is,*ic,*ir;

    std::tie(Lv,iv) = a.at(nIv);
    std::tie(Ls,is) = a.at(nIs);
    std::tie(Lc,ic) = a.at(nIc);
    std::tie(Lr,ir) = a.at(nIr);

    llvm::Value * Iv = llvm_create_value(Lv);
    llvm::Value * Is = llvm_create_value(Ls);
    llvm::Value * Ic = llvm_create_value(Lc);
    llvm::Value * Ir = llvm_create_value(Lr);

    llvm::Value * iv_div_inner = llvm_div( iv , inner );
    llvm::Value * iv_mod_inner = llvm_rem( iv , inner );

    // offset = ((ir * Ic + ic) * Is + is) * Iv + iv

    if (lay == JitDeviceLayout::LayoutCoalesced) {
      return llvm_add(llvm_mul(llvm_add(llvm_mul( llvm_add(llvm_mul(llvm_add(llvm_mul(iv_div_inner,Is),is),Ic),ic),Ir),ir),inner),iv_mod_inner);
    } else {
      return llvm_add(llvm_mul(llvm_add(llvm_mul( llvm_add(llvm_mul(iv,Is),is),Ic),ic),Ir),ir);
    }
  }
#endif



#if 0
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    const size_t nIv = 0; // volume
    const size_t nIs = 1; // spin
    const size_t nIc = 2; // color
    const size_t nIr = 3; // reality

    int         Lv,Ls,Lc,Lr;
    llvm::Value *iv,*is,*ic,*ir;

    std::tie(Lv,iv) = a.at(nIv);
    std::tie(Ls,is) = a.at(nIs);
    std::tie(Lc,ic) = a.at(nIc);
    std::tie(Lr,ir) = a.at(nIr);

    llvm::Value * Iv = llvm_create_value(Lv);
    llvm::Value * Is = llvm_create_value(Ls);
    llvm::Value * Ic = llvm_create_value(Lc);
    llvm::Value * Ir = llvm_create_value(Lr);

    // offset = ((ir * Ic + ic) * Is + is) * Iv + iv

    if (lay == JitDeviceLayout::LayoutCoalesced) {
      return llvm_add(llvm_mul(llvm_add(llvm_mul( llvm_add(llvm_mul(ir,Ic),ic),Is),is),Iv),iv);
    } else
      return llvm_add(llvm_mul(llvm_add(llvm_mul( llvm_add(llvm_mul(iv,Is),is),Ic),ic),Ir),ir);
  }
#endif


#if 0
  llvm::Value * datalayout_stack( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    assert(a.size() > 0);
    llvm::Value * offset = llvm_create_value(0);
    for( auto x = a.rbegin() ; x != a.rend() ; x++ ) {
      int         Index;
      llvm::Value * index;
      std::tie(Index,index) = *x;
      llvm::Value * Index_jit = llvm_create_value(Index);
      offset = llvm_add( llvm_mul( offset , Index_jit ) , index );
    }
    return offset;
  }
#endif


#if 0
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    assert(a.size() > 0);

    // In case of a coalesced layout (OLattice)
    // We reverse the data layout given by the natural nesting order
    // of aggregates, i.e. reality slowest, lattice fastest
    // In case of a scalar layout (sums,comms buffers,OScalar)
    // We actually use the index order/data layout given by the
    // nesting order of aggregates
    if ( lay == JitDeviceLayout::LayoutCoalesced ) {
      //QDPIO::cerr << "not applying special data layout\n";
      std::reverse( a.begin() , a.end() );
    }

    llvm::Value * offset = llvm_create_value(0);
    for( auto x = a.begin() ; x != a.end() ; x++ ) {
      int         Index;
      llvm::Value * index;
      std::tie(Index,index) = *x;
      llvm::Value * Index_jit = llvm_create_value(Index);
      offset = llvm_add( llvm_mul( offset , Index_jit ) , index );
    }
    return offset;
  }
#endif


} // namespace
