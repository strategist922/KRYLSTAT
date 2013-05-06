#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET

#include <omp.h>
#include <Eigen/Sparse>
#include <iostream>
#include "eigen_cocg_m.h"
#include "eigen_basic_krylov.h"
#include "../../ratApps/sncndn.h"
#include "../../timer.h"

//#include <fftw3.h>
// #include <time.h>

using namespace Eigen;

int main()
{
  int_type sizeAmat=10000;
  int_type nshifts=4;
  SparseMatrix<double> A(sizeAmat,sizeAmat);
  A.reserve(3*sizeAmat);
  for (int iii=0;iii<sizeAmat;++iii){A.insert(iii,iii)=2.001;}
  for (int iii=0;iii<sizeAmat-1;++iii){A.insert(iii+1,iii)=-1.;}
  for (int iii=0;iii<sizeAmat-1;++iii){A.insert(iii,iii+1)=-1.;}
  A.finalize();
  
  Vd b=Vd::Random(sizeAmat);
  
  Vcpx cpxShifts(nshifts); for(int iii=0;iii<nshifts;++iii){cpxShifts(iii)=cpxDbl(0.1*(double)(iii+1),0.1*(double)(iii+1));}
  Vd dShifts(nshifts); for(int iii=0;iii<nshifts;++iii){dShifts(iii)=0.1*(double)(iii+1);}
  
  default_monitor<double> M1(b);
  
  Vd x(sizeAmat);
  Mcpx cpx_x_sh(sizeAmat,nshifts);
  Md d_x_sh(sizeAmat,nshifts);

  
  timer tim;
  tim.start("Timer started");
  eigen_cocg_m(A,b,x,cpx_x_sh,cpxShifts,M1);
  std::cout << "Complex shifts took: " << tim << "\n";
  
  default_monitor<double> M2(b);
  

  eigen_cocg_m(A,b,x,d_x_sh,dShifts,M2);
  
  
  
  std::cout << "Complex converged: "<< M1.converged() << ".  True residual is:  " << (A*cpx_x_sh.col(1)+cpxShifts(1)*cpx_x_sh.col(1)-b.cast<cpxDbl>()).norm() << "\n";
  std::cout << "Double converged: "<< M2.converged() << ".  True residual is:  " << (A*d_x_sh.col(3)+dShifts(3)*d_x_sh.col(3)-b).norm() << "\n";

  x=0.*x;
  cpx_x_sh=cpxDbl(0.)*cpx_x_sh;
  default_monitor<double> M3(b);
  
  tim.restart("Timer restarted");
  eigen_cocg_m(A,b,x,cpx_x_sh,cpxShifts,M3);
  std::cout << "Double shifts alternative took: " << tim << "\n";
  
  std::cout << "Double converged: "<< M3.converged() << ".  True residual is:  " << (A*d_x_sh.col(3)+dShifts(3)*d_x_sh.col(3)-b).norm() << "\n";
  
  /* xxxxxxxx
  std::cout << "Testing different types: \n";
  
  Matrix<cpxDbl,Dynamic,1> bbb=VectorXcd::Random(10);
  Matrix<double,Dynamic,Dynamic> aaa=Matrix<double,Dynamic,Dynamic>::Random(10,2);
  std::cout << bbb << "\n \n" << aaa << "\n\n" << (typeid(d_x_sh(1,1))==typeid(cpx_x_sh(1,1))) 
	    << "\n\n" << cpx_x_sh << "\n\n" << d_x_sh << "\n\n";
  xxxxxxxxxx */
  
  
}