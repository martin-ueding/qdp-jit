#include "qdp.h"

#if defined(QDP_USE_OMP_THREADS)

#include <omp.h>
namespace QDP {    
  
  int qdpNumThreads() {
    return omp_get_max_threads();
  }

  void qdpSetNumThreads(int n) {
    omp_set_num_threads(n);
  }

  void jit_dispatch( void* function , const AddressLeaf& args)
  {
    void (*FP)(void*) = (void (*)(void*))(intptr_t)function;
    
    // int threads_num;
    // int64_t myId;
    // int64_t lo = 0;
    // int64_t hi = site_count;
    void * addr = args.addr.data();

    QDPIO::cerr << "argument array size " << args.addr.size() << "\n";

    QDPIO::cerr << "dispatch...\n";

    FP( addr );

// #pragma omp parallel shared(site_count, threads_num, ordered, start, addr) private(myId, lo, hi) default(shared)
//     {
//       threads_num = omp_get_num_threads();
//       myId = omp_get_thread_num();
//       lo = ((site_count/inner)*myId/threads_num)*inner;
//       hi = ((site_count/inner)*(myId+1)/threads_num)*inner;

//       //std::cout << "dispatch: " << lo << " " << hi << " " << myId << " " << ordered << " " << start << "\n";

//       FP( lo , hi , myId , ordered, start, addr );
//     }

    QDPIO::cerr << "         (returned)\n";
  }


}

#else 

#if defined(QDP_USE_QMT_THREADS)
#error "QMT threading not implemented"
#endif

#error "Must use threading"

#endif

