#ifndef QDP_JITF_COPYMASK_H
#define QDP_JITF_COPYMASK_H

#include "qmp.h"

namespace QDP {

  template<class T,class T1>
  void
  function_copymask_build( JitFunction& func, OLattice<T>& dest , const OLattice<T1>& mask , const OLattice<T>& src )
  {
    llvm_start_new_function();

    ParamLeaf param_leaf;

    typedef typename LeafFunctor<OLattice<T>, ParamLeaf>::Type_t  FuncRet_t;
    typedef typename LeafFunctor<OLattice<T1>, ParamLeaf>::Type_t  FuncRet1_t;

    FuncRet_t dest_jit(forEach(dest, param_leaf, TreeCombine()));
    FuncRet_t src_jit(forEach(src, param_leaf, TreeCombine()));
    FuncRet1_t mask_jit(forEach(mask, param_leaf, TreeCombine()));

    typedef typename REGType<typename FuncRet_t::Subtype_t>::Type_t REGFuncRet_t;
    typedef typename REGType<typename FuncRet1_t::Subtype_t>::Type_t REGFuncRet1_t;

    for ( int vol = 0 ; vol < Layout::sitesOnNode() ; ++vol ) {

      std::array<int,Nd> coord = volume_loop_linear_2_coord(vol);

      IndexDomainVector idx;
      for( int i = 0 ; i < Nd ; ++i )
	idx.push_back( make_pair( Layout::subgridLattSize()[i] , coord[i] ) );

      REGFuncRet_t src_reg;
      REGFuncRet1_t mask_reg;
      src_reg.setup ( src_jit.elem( JitDeviceLayout::LayoutCoalesced , idx ) );
      mask_reg.setup( mask_jit.elem( JitDeviceLayout::LayoutCoalesced , idx ) );

      copymask( dest_jit.elem( JitDeviceLayout::LayoutCoalesced , idx ) , mask_reg , src_reg );
    }

    func.func().push_back( jit_function_epilogue_get("jit_copymask.ptx") );
  }



  template<class T,class T1>
  void 
  function_copymask_exec(const JitFunction& function, OLattice<T>& dest, const OLattice<T1>& mask, const OLattice<T>& src )
  {
    AddressLeaf addr_leaf(all);

    int junk_0 = forEach(dest, addr_leaf, NullCombine());
    int junk_1 = forEach(src, addr_leaf, NullCombine());
    int junk_2 = forEach(mask, addr_leaf, NullCombine());

#ifdef LLVM_DEBUG
    std::cout << "calling copymask(Lattice)..\n";
#endif

    //jit_dispatch(function.func().at(0),th_count,getDataLayoutInnerSize(),true,0,addr_leaf);
    jit_dispatch(function.func().at(0),addr_leaf);
  }

}
#endif
