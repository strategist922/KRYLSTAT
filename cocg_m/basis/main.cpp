#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET

#include <omp.h>
#include <Eigen/Sparse>
#include <iostream>
#include "cocg_m.h"
#include <fftw3.h>


int main()
{
  Eigen::SparseMatrix<float> m1(100,100);  
  Eigen::SparseVector<float> v1(100);
  Eigen::Matrix<std::complex<float>,100,1> vx;
  Eigen::Matrix<std::complex<float>,100,2> v2;
  
  std::cout << v2.rows() << ", " << v2.cols() << "\n";
  std::cout << v1.rows() << ", " << v1.cols() << "\n";
  
  v2.col(1)=m1*vx;
  
  Eigen::VectorXd d=Eigen::MatrixXd::Ones(10,1);
  std::cout << d(1) << "\n";
  
  Eigen::VectorXd randvec1=Eigen::MatrixXd::Random(10,1);
  Eigen::VectorXd randvec2=Eigen::MatrixXd::Random(10,1);
  std::cout << randvec1 << "\n";
  std::cout << randvec1.size() << "\n"; 
  std::cout << isnan(0./0.) << "\n";
  
  std::cout << "\n \n" << "Beginning to test cocg_m \n";
  
  int sizeAmat=10;
  int nshifts=4;
  Eigen::SparseMatrix<double> A(sizeAmat,sizeAmat);
  A.reserve(3*sizeAmat);
  for (int iii=0;iii<sizeAmat;++iii){A.insert(iii,iii)=2.001;}
  for (int iii=0;iii<sizeAmat-1;++iii){A.insert(iii+1,iii)=-1.;}
  for (int iii=0;iii<sizeAmat-1;++iii){A.insert(iii,iii+1)=-1.;}
  Eigen::VectorXd b=Eigen::MatrixXd::Random(sizeAmat,1);
  Eigen::VectorXd x=Eigen::MatrixXd::Zero(sizeAmat,1);
  Eigen::VectorXd shifts(nshifts); for(int iii=0;iii<nshifts;++iii){shifts(iii)=0.001*(double)(iii+1);}
  Eigen::MatrixXd x_sh=Eigen::MatrixXd::Zero(sizeAmat,nshifts);
  default_monitor<ValueType> mon(b,500,1.e-2);
 
  
  
  // Eigen::MatrixXd bs=Eigen::MatrixXd::Random(sizeAmat,10);
  // Eigen::MatrixXd xs=Eigen::MatrixXd::Zero(sizeAmat,10);  
  
  //cocg_m(A,bs.col(0),xs.col(0),x_sh,shifts,mon);
  
  // std::cout << A*bs.col(9) << "\n";
  
/*#pragma omp parallel for
  for(int iii=0;iii<10;++iii)
  {
    Eigen::VectorXd temp1=Eigen::MatrixXd::Random(sizeAmat,1);
    Eigen::VectorXd temp2=Eigen::MatrixXd::Zero(sizeAmat,1);
    default_monitor<ValueType> mono(temp1);
    cocg_m(A,temp1,temp2,x_sh,shifts,mono);
    xs.col(iii)=temp2;
  }*/
  
  std::cout << "\n \n" << "Testing if shifted version works \n";
  cocg_m(A,b,x,x_sh,shifts,mon);
  std::cout << "Converged: "<< mon.converged() << ".  True residual is:  " << (A*x_sh.col(3)+shifts(3)*x_sh.col(3)-b).norm() << "\n";
  
  std::cout << "\n \n" << "Testing complex shifts \n";
  
  Eigen::VectorXcd cpxShifts=Eigen::MatrixXcd::Zero(nshifts,1);
  for(int iii=0;iii<nshifts;++iii){cpxShifts(iii)=cpxDbl(0.001*double(iii),0.001*double(iii));}
  
  std::cout << "Complex shifts are: " << cpxShifts  << "\n";
  Eigen::MatrixXcd x_sh_cpx=Eigen::MatrixXcd::Zero(sizeAmat,nshifts);
  default_monitor<ValueType> mon_cpx(b,500,1.e-2);
  std::cout << x_sh.cast<float>() << "\n";
  
  // std::cout << A*Eigen::MatrixXcd::Random(sizeAmat,1) << "\n";
  
  // std::cout << "Converged: "<< mon_cpx.converged() << ".  True residual is:  " << (A*x_sh_cpx.col(3)+cpxShifts(3)*x_sh_cpx.col(3)-b).norm() << "\n";
  
  

  return 1;
}