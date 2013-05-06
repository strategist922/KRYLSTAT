// This file is part of the KRYLSTAT function library
//
// Copyright (C) 2011 Erlend Aune <erlenda@math.ntnu.no>
//
// The KRYLSTAT library is free software; 
// you can redistribute it and/or modify it under the terms 
// of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the 
// License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// The KRYLSTAT library is distributed in the 
// hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License
// or the GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// the KRYLSTAT library. If not, see 
// <http://www.gnu.org/licenses/>.

#ifndef EIGEN_NESTED_SIGN_H
#define EIGEN_NESTED_SIGN_H




template <class LinearOperator, class Monitor, class Derived1, class EigScal>
void eigen_nested_sign(const LinearOperator &A, const MatrixBase<Derived1> &b,
		   const MatrixBase<Derived1> &x, const MatrixBase<Derived1> &defVecs, Monitor &mon)
{
  typedef typename Derived1::Scalar Scalar;
  typedef std::complex<Scalar> cpxScal;
  
  assert(A.cols() == A.rows());
  assert(A.cols() == b.rows());
  //std::cout << A.cols() << "\n";
  //int N=A.cols();
  
  Derived1 r=b-A*x;
  Derived1 p=r;
  Derived1 ap=Derived1::Zero(A.cols());
  
  Scalar b_norm=b.norm();
  Scalar c_cur=b_norm*b_norm;
  
  Scalar b_cur;
  Scalar b_old;
  Scalar a_cur;
  Scalar a_old;
  Scalar c_new;
  
  std::vector<Scalar> delta;
  std::vector<Scalar> eta;
  delta.reserve(mon.iteration_limit());
  eta.reserve(mon.iteration_limit());
  
  bool finish_flag=0;
  
  while(!finish_flag)
  {
    /*! Cg iterations */
    ap=A*p;
//    if (mon.iteration_count()==1)
//     {
//       std::cout << "Correct size" << ap.size()<< "\n";
//       
//     }
    a_cur=c_cur/ap.dot(p);
    
    const_cast< MatrixBase<Derived1>& >(x)=x+a_cur*p;
    r=r-a_cur*ap;
    
    c_new=r.dot(r);
    b_cur=c_new/c_cur;
    p=r+b_cur*p;
    /*! Cg iterations end*/
    
    
    /*! Lanczos T matrix */
    if (mon.iteration_count() == 0)
    {
      delta.push_back(1./a_cur);
    }
    else
    {
      delta.push_back(1./a_cur+b_old/a_old);
    }
    /*! End Lanczos T matrix */
    
      
    
    c_cur=c_new;
    a_old=a_cur;
    b_old=b_cur;
    
    finish_flag=mon.finished(r);
    eta.push_back(std::sqrt(b_cur)/a_cur);
    
    ++mon;
  }
  
  std::cout << mon.residual_norm() << "\n\n";
  // std::cout << eta[1] << "\n";
  
  /*! Eigensystem tridiagonal */
  alglib::real_1d_array diag;
  alglib::real_1d_array subdiag;
//   alglib::real_2d_array minEigvec;
//   alglib::real_2d_array maxEigvec;
  alglib::real_2d_array eigvecs;
//  eigvecs.setlength(delta.size(),delta.size());
  
  
  diag.setcontent(delta.size(),&delta[0]);
  subdiag.setcontent(eta.size()-1,&eta[0]);
  
  int type=0;
  bool success=0;
  
  success=alglib::smatrixtdevd(diag, subdiag, delta.size(), type, eigvecs);
  
  minEig=diag(0);
  maxEig=diag(delta.size()-1);
  
  minEig=0.8*minEig;
  maxEig=1.2*maxEig;
  /*! End Eigensystem tridiagonal */
  
  std::cout << "Success: " << success << " Estimates: minEig=" << minEig << ", maxEig=" << maxEig << " Iterations: " << mon.iteration_count() 
    << " RelError: " << (A*x-b).norm()/b.norm() << "\n";
}


#endif