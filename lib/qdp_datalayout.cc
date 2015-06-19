#include "qdp.h"

#include <array>

namespace QDP {


  // These functions convert a linear space-time index into a index vector
  // as used for the coalesced memory accesses.


  int jit_local_site(const std::array<int,Nd>& coord, const multi1d<int>& latt_size)
  {
    int order = 0;

    for(int mmu=latt_size.size()-1; mmu >= 1; --mmu)
      order = latt_size[mmu-1]*(coord[mmu] + order);

    order += coord[0];

    return order;
  }



#if 1
  //
  // Datalayout which is SIMD friendly for shift operations
  //
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    assert( a.size() == 3+Nd && "IndexDomainVector size not 3+Nd" );

    std::array<int,Nd> lc;  // coordinate within a subnode
    std::array<int,Nd> sn;  // coordinate of the subnode

    for(int i=0;i<Nd;++i) {
      lc[i] = a[i].second % Layout::subnodeLattSize()[i];
      sn[i] = a[i].second / Layout::subnodeLattSize()[i];
      //      QDPIO::cout << coord[i] << " ";
    }
    //    QDPIO::cout << "\n";

    int inner_idx = jit_local_site( sn , Layout::nodeGeom() );
    int outer_idx = jit_local_site( lc , Layout::subnodeLattSize() );

    int inner_domain = Layout::jit_get_number_of_subnodes_per_node();

    int space_time_index = inner_domain * outer_idx + inner_idx;


    int order = 0;

    for( int mmu = a.size() - 1 ; mmu >= Nd+1 ; --mmu ) {
      order = a[mmu-1].first*(a[mmu].second + order);
    }
    order += a[Nd].second;

    QDPIO::cout << "order: " << order << "    sti = " << space_time_index << "\n";

    order = order * Layout::sitesOnNode() + space_time_index;


    return llvm_create_value( order );
  }
#endif



#if 0
  llvm::Value * datalayout( JitDeviceLayout::LayoutEnum lay , IndexDomainVector a ) {
    assert( a.size() == 7 && "IndexDomainVector size not 7" );

    std::size_t ret = 0;
    for( IndexDomainVector::reverse_iterator x = a.rbegin() ; x != a.rend() ; x++ ) {
      int domain = x->first;
      int index  = x->second;
      ret *= domain;
      ret += index;
    }
    return llvm_create_value( ret );
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



  std::vector< std::array<int,Nd> > jit_volume_loop;

  std::array<int,Nd> volume_loop_linear_2_coord( int linear )
  {
    QDPIO::cout << linear << " " << jit_volume_loop.size() << "\n";
    assert( linear < jit_volume_loop.size() );
    assert( jit_volume_loop.size() > 0 );
    return jit_volume_loop[linear];
  }


  bool lexico_percolate( std::array<int,Nd>& coord, const multi1d<int>& bounds )
  {
    bool ret=false;
    for( int i = 0 ; i < Nd ; ++i )
      if (coord[i] == bounds[i]) {
	coord[i] = 0;
	if ( i < Nd-1 )
	  coord[i+1]++;
	else {
	  coord.fill(0);
	  ret=true;
	}
      }
    return ret;
  }


  void setup_nodevolume_loop_SIMD()
  {
    QDPIO::cout << "Setting up node volume loop, SIMD friendly version\n";
    
    std::array<int,Nd> lc;  // coordinate within a subnode
    std::array<int,Nd> sn;  // coordinate of the subnode
    lc.fill(0);
    sn.fill(0);

    jit_volume_loop.clear();
    int vol(0);

    do
      {
	if (lexico_percolate( sn , Layout::nodeGeom()) ) {
	  lc[0]++;
	  if (lexico_percolate( lc , Layout::subnodeLattSize() ))
	    QDP_error_exit("Didn't expect the subnode coordinate to percolate.");
	}

	std::array<int,Nd> coord;

	for(int i=0;i<Nd;++i) {
	  coord[i] = sn[i] * Layout::subnodeLattSize()[i] + lc[i];
	  QDPIO::cout << coord[i] << " ";
	}
	QDPIO::cout << "\n";

	// Paranoic test
	for (auto& s : jit_volume_loop)
	  if ( s == coord )
	    QDP_error_exit("Double coordinate");

	jit_volume_loop.push_back( coord );

	sn[0]++;
	vol++;
	//QDPIO::cout << vol << " " << Layout::sitesOnNode() << "\n";
      } 
    while (vol <= Layout::sitesOnNode()-1);

    QDPIO::cout << "Done\n";  

  } //


} // namespace
