#ifndef QDP_LAYOUTFUNC_H
#define QDP_LAYOUTFUNC_H

namespace QDP {


  struct LayoutFunc {
    LayoutFunc() {}
    LayoutFunc( int limit , int value ) { 
      lim.push_back(limit);
      val.push_back(value);
      //std::cout << "LayoutFunc(int,int) lim_size=" << lim.size() << " val_size=" << val.size() << "\n";
    }
    LayoutFunc(const LayoutFunc& func) : lim(func.lim),val(func.val) {
      //std::cout << "LayoutFunc(const LayoutFunc& func) lim_size=" << lim.size() << " val_size=" << val.size() << "\n";
    }
    LayoutFunc curry( int limit , int value ) const {
      //std::cout << "curry " << limit << " " << value << " lim_size=" << lim.size() << " val_size=" << val.size() << "\n";
      std::vector<int> rlim(lim);
      std::vector<int> rval(val);
      rlim.push_back(limit);
      rval.push_back(value);
      LayoutFunc ret(rlim,rval);
      return ret;
    };
    LayoutFunc& operator=(const LayoutFunc& rhs) {
      //std::cout << "LayoutFunc op=\n";
      lim = rhs.lim;
      val = rhs.val;
    }
    LayoutFunc(const std::vector<int>& lim_, 
	       const std::vector<int>& val_ ) : lim(lim_),val(val_) {}
    int getOffset() const {
      int off=0;
      int full=1;
      for (int i=0 ; i<lim.size() ; i++) {
	//std::cout << "i=" << i << " val=" << val[i] << " lim=" << lim[i] << "\n";
	off += val[i] * full;
	full *= lim[i];
      }
      return off;
    }

    std::vector<int> lim;
    std::vector<int> val;
  private:
  };

}

#endif
