// $Id: t_blas_g5_3.cc,v 1.3 2005-06-16 12:01:10 bjoo Exp $

#include <iostream>
#include <cstdio>

// Ensure that the template matches don't get used
#include "qdp.h"


#include "scalarsite_generic/generic_blas_g5.h"
 
using namespace QDP;

int main(int argc, char *argv[])
{
  // Put the machine into a known state
  QDP_initialize(&argc, &argv);

  // Setup the layout
  const int foo[] = {8,8,8,8};
  multi1d<int> nrow(Nd);
  nrow = foo;  // Use only Nd elements
  Layout::setLattSize(nrow);
  Layout::create();

  
  Real a=Real(1.5);
  Real b=Real(2.0);
  
  LatticeFermion x;
  LatticeFermion y;
  LatticeFermion z1;
  LatticeFermion z2;
  LatticeFermion tmp;
  int G5=Ns*Ns-1;

  gaussian(x);
  gaussian(y);




  REAL* Out;
  REAL* scalep;
  REAL* scalep2;
  REAL* InScale;
  REAL* Add;

  int n_4vec = all.end()-all.start()+1;
  Double norm_diff;

  gaussian(x);
  tmp=Gamma(G5)*x;
  z1 = a*tmp;

  Out = (REAL *)&(z2.elem(0).elem(0).elem(0).real());
  scalep = (REAL *)&(a.elem().elem().elem().elem());
  InScale = (REAL *)&(x.elem(0).elem(0).elem(0).real());

  // z2 = a*g5*x
  z2=a*(GammaConst<Ns,Ns*Ns-1>()*x);

  norm_diff=norm2(z1-z2);
 
  {
    QDPIO::cout << "ag5x diff=" << sqrt(norm_diff) << endl;
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = a*tmp;	
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      fc.reset(); swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = a*tmp;
	fc.addSiteFlops(2*Nc*Ns, all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report("ag5x", Real(time));

      //double Nflops = (double)(2*Nc*Ns*Layout::sitesOnNode()*iter);
      //QDPIO::cout << "Time taken: " << time << "(us) Perf: " << Nflops/time << " Mflop/s per node" << endl;
    }
  }

  {
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset(); 
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2=a*(GammaConst<Ns,Ns*Ns-1>()*x);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      swatch.reset(); fc.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2=a*(GammaConst<Ns,Ns*Ns-1>()*x);
	fc.addSiteFlops(2*Nc*Ns,all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report("new ag5x", Real(time));
    }
  }

  // ax + bg5y
  gaussian(x);
  gaussian(y);
  tmp=Gamma(G5)*x;
  z1 = b*y + a*tmp;
  z2=b*y + a*(GammaConst<Ns,Ns*Ns-1>()*x);  
  norm_diff=norm2(z1-z2); 
  {
    QDPIO::cout << "ax + bg5y diff=" << sqrt(norm_diff) << endl;
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = b*y + a*tmp;	
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {    
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = b*y + a*tmp;	
	fc.addSiteFlops(3*2*Nc*Ns,all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("old axpbg5y"), Real(time));
    }
  }

  {
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2=a*y + b*(GammaConst<Ns,Ns*Ns-1>()*x);  
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }

    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      fc.reset();
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2=a*y + b*(GammaConst<Ns,Ns*Ns-1>()*x);
	fc.addSiteFlops(6*Nc*Ns);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("axpbg5y"), Real(time));
    }
    
  }
    // x - ag5y
  gaussian(x);
  gaussian(y);
  tmp=Gamma(G5)*x;
  z1 = y - a*tmp;
  
  z2 = y - a*(GammaConst<Ns,Ns*Ns-1>()*x);

  norm_diff=norm2(z1-z2);
 
  {
    QDPIO::cout << "x - a g5 y diff=" << sqrt(norm_diff) << endl;
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = y - a*tmp;	
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    
    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      fc.reset();
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=Gamma(G5)*x;
	z1 = y - a*tmp;	
	fc.addSiteFlops(4*Nc*Ns,all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("old xmag5y"), Real(time));
    }
  }

  {
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2 = y - a*(GammaConst<Ns,Ns*Ns-1>()*x);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {    
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      fc.reset();
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2 = y - a*(GammaConst<Ns,Ns*Ns-1>()*x);
	fc.addSiteFlops(4*Nc*Ns, all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("new xmag5y"), Real(time));
    }
  }



  // g5(ax - by)

  gaussian(x);
  gaussian(y);
  tmp=a*x-b*y;
  z1 = GammaConst<Ns,Ns*Ns-1>()*tmp;
  z2 = GammaConst<Ns,Ns*Ns-1>()*(a*x-b*y);
  norm_diff=norm2(z1-z2);
 
  {
    QDPIO::cout << "g5( ax - by) diff=" << sqrt(norm_diff) << endl;
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=a*x-b*y;
	z1 = GammaConst<Ns,Ns*Ns-1>()*tmp;
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	tmp=a*x-b*y;
	z1 = GammaConst<Ns,Ns*Ns-1>()*tmp;
	fc.addSiteFlops(6*Nc*Ns, all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("old g5_axmby"), Real(time));
      //      double Nflops = (double)(6*Nc*Ns*Layout::sitesOnNode()*iter);
      //QDPIO::cout << "Time taken: " << time << "(us) Perf: " << Nflops/time << " Mflop/s per node" << endl;
    }
  }
  {
    StopWatch swatch;
    double time=0;
    int iter=1;
    while( time < 1.0 ) { 
      iter *=2;
      QDPIO::cout << "Calling " << iter << " times " << endl;
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2 = GammaConst<Ns,Ns*Ns-1>()*(a*x-b*y);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
    }
    {
      QDPIO::cout << "Timing with " << iter << " iters" << endl;
      FlopCounter fc;
      fc.reset();
      swatch.reset();
      swatch.start();
      for(int i=0; i < iter; i++) {
	z2 = GammaConst<Ns,Ns*Ns-1>()*(a*x-b*y);
	fc.addSiteFlops(6*Nc*Ns,all);
      }
      swatch.stop();
      time = swatch.getTimeInSeconds();
      Internal::broadcast(time);
      fc.report(std::string("new g5_axmby"), Real(time));
		//      double Nflops = (double)(6*Nc*Ns*Layout::sitesOnNode()*iter);
		//QDPIO::cout << "Time taken: " << time << "(us) Perf: " << Nflops/time << " MFlops/node" << endl;
    }
  }


  // Time to bolt
  QDP_finalize();

  exit(0);
}
  
