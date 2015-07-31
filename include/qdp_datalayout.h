// -*- C++ -*-

#ifndef QDP_DATALAYOUT_H
#define QDP_DATALAYOUT_H

#include "qdp.h"
#include "qdp_params.h"

namespace QDP {

  void QDP_set_jit_datalayout(int pos_o, int pos_s, int pos_c, int pos_r, int pos_i);
  void QDP_print_jit_datalayout();
  //llvm::Value * datalayout_stack(IndexDomainVector a);

  void set_datalayout_packed();
  void set_datalayout_packed_create();

  void set_datalayout_subnode();

  // void setup_nodevolume_loop_SIMD();
  // std::array<int,Nd> volume_loop_linear_2_coord( int linear );
  //std::array<int,Nd> volume_loop_linear_2_coord( int linear );

} // namespace QDP

#endif
