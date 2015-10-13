#ifndef QDP_JITF_LAYOUT_H
#define QDP_JITF_LAYOUT_H

#include "qmp.h"

namespace QDP {

  template<class T>
  void 
  function_layout_to_jit_build( JitFunction& func, const OLattice<T>& dest )
  {
    QDPIO::cerr << __PRETTY_FUNCTION__ << "\n";
    //assert( 0 && "ni");
#if 1
    llvm_start_new_function();

    ParamLeaf param_leaf;

    typedef typename LeafFunctor<OLattice<T>, ParamLeaf>::Type_t  FuncRet_t;

    FuncRet_t dest_jit(forEach(dest, param_leaf, TreeCombine()));
    FuncRet_t src_jit(forEach(dest, param_leaf, TreeCombine()));

    typedef typename REGType<typename FuncRet_t::Subtype_t>::Type_t REGFuncRet_t;

    for ( int vol = 0 ; vol < Layout::sitesOnNode() ; ++vol ) {
      //std::array<int,Nd> coord = volume_loop_linear_2_coord(vol);
      multi1d<int> coord = crtesn( vol , Layout::subgridLattSize() );

      IndexDomainVector idx;
      for( int i = 0 ; i < Nd ; ++i )
	idx.push_back( make_pair( Layout::subgridLattSize()[i] , coord[i] ) );

      REGFuncRet_t src_reg;
      src_reg.setup ( src_jit.elem( JitDeviceLayout::LayoutScalar , idx ) );

      dest_jit.elem( JitDeviceLayout::LayoutCoalesced , idx ) = src_reg;

    }

    func.func().push_back( jit_function_epilogue_get("jit_layout.ptx") );
#endif
  }



  template<class T>
  void 
  function_layout_to_jit_exec(const JitFunction& function, T *dest, T *src )
  {
    QDPIO::cerr << __PRETTY_FUNCTION__ << "\n";
    AddressLeaf addr_leaf(all);

    addr_leaf.setAddr( dest );
    addr_leaf.setAddr( src );

    int th_count = Layout::sitesOnNode();

    QDPIO::cerr << "calling layout(to JIT)..\n";

    //assert( 0 && "ni");
    //jit_dispatch(function.func().at(0),th_count,getDataLayoutInnerSize(),true,0,addr_leaf);
    jit_dispatch(function.func().at(0),addr_leaf);
  }










  template<class T>
  void 
  function_layout_to_native_build( JitFunction& func, const OLattice<T>& dest )
  {
#if 1
    QDPIO::cerr << __PRETTY_FUNCTION__ << "\n";
    llvm_start_new_function();

    ParamLeaf param_leaf;

    typedef typename LeafFunctor<OLattice<T>, ParamLeaf>::Type_t  FuncRet_t;

    FuncRet_t dest_jit(forEach(dest, param_leaf, TreeCombine()));
    FuncRet_t src_jit(forEach(dest, param_leaf, TreeCombine()));

    typedef typename REGType<typename FuncRet_t::Subtype_t>::Type_t REGFuncRet_t;

    for ( int vol = 0 ; vol < Layout::sitesOnNode() ; ++vol ) {
      //std::array<int,Nd> coord = volume_loop_linear_2_coord(vol);
      multi1d<int> coord = crtesn( vol , Layout::subgridLattSize() );

      IndexDomainVector idx;
      for( int i = 0 ; i < Nd ; ++i )
	idx.push_back( make_pair( Layout::subgridLattSize()[i] , coord[i] ) );

      REGFuncRet_t src_reg;
      src_reg.setup ( src_jit.elem( JitDeviceLayout::LayoutCoalesced , idx ) );

      dest_jit.elem( JitDeviceLayout::LayoutScalar , idx ) = src_reg;

    }

    //QDPIO::cerr << "functionlayout_to_native_build\n";

    func.func().push_back( jit_function_epilogue_get("jit_layout.ptx") );
#endif
  }



  template<class T>
  void 
  function_layout_to_native_exec( const JitFunction& function, T *dest, T *src )
  {
    QDPIO::cerr << __PRETTY_FUNCTION__ << "\n";
    AddressLeaf addr_leaf(all);

    addr_leaf.setAddr( dest );
    addr_leaf.setAddr( src );

    int th_count = Layout::sitesOnNode();

    QDPIO::cerr << "calling layout(to native)..\n";

    //assert( 0 && "ni");
    //jit_dispatch(function.func().at(0),th_count,getDataLayoutInnerSize(),true,0,addr_leaf);
    jit_dispatch(function.func().at(0),addr_leaf);
  }

}
#endif
