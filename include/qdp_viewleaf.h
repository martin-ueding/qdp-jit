#ifndef QDP_VIEWLEAF
#define QDP_VIEWLEAF

namespace QDP {

template<class T>
struct LeafFunctor<QDPTypeJIT<T,OLatticeJIT<T> >, ViewLeaf>
{
  typedef typename REGType<T>::Type_t Type_t;
  inline static
  Type_t apply(const QDPTypeJIT<T,OLatticeJIT<T> > & s, const ViewLeaf& v)
  {
    Type_t reg;
    reg.setup( s.elem( v.getLayout() , v.getIndexVec() ) );
    return reg;
  }
};


template<class T>
struct LeafFunctor<QDPTypeJIT<T,OScalarJIT<T> >, ViewLeaf>
{
  typedef typename REGType<T>::Type_t Type_t;
  inline static
  Type_t apply(const QDPTypeJIT<T,OScalarJIT<T> > & s, const ViewLeaf& v)
  {
    IndexDomainVector idx0;
    for( int i = 0 ; i < Nd ; ++i ) {
      idx0.push_back( make_pair( Layout::subgridLattSize()[i] , 0 ) );
    }

    Type_t reg;
    reg.setup( s.elem( v.getLayout() , idx0 ) );
    //reg.setup( s.elem() );
    return reg;
  }
};


template<class T>
struct LeafFunctor<OScalarJIT<T>, ViewLeaf>
{
  typedef typename REGType<T>::Type_t Type_t;
  inline static
  Type_t apply(const OScalarJIT<T> & s, const ViewLeaf& v)
  {
    IndexDomainVector idx0;
    for( int i = 0 ; i < Nd ; ++i ) {
      idx0.push_back( make_pair( Layout::subgridLattSize()[i] , 0 ) );
    }

    Type_t reg;
    reg.setup( s.elem( v.getLayout() , idx0 ) );
    //reg.setup( s.elem() );
    return reg;
  }
};

}

#endif
