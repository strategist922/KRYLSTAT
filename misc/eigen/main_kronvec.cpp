

#include "../../timer.h"
#include "kronMatVec.h"
#include <Eigen/StdVector>

int main()
{
  int sizeAmat=500;
  SparseMatrix<double,1,int> AB(sizeAmat,sizeAmat);
  AB.reserve(3*sizeAmat);
  for (unsigned int iii=0;iii<sizeAmat;++iii){AB.insert(iii,iii)=2.01;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB.insert(iii+1,iii)=-1.;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB.insert(iii,iii+1)=-1.;}
  AB.finalize();
  
  sizeAmat=499;
  SparseMatrix<double,1,int> AB2(sizeAmat,sizeAmat);
  AB2.reserve(3*sizeAmat);
  for (unsigned int iii=0;iii<sizeAmat;++iii){AB2.insert(iii,iii)=2.01;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB2.insert(iii+1,iii)=-1.;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB2.insert(iii,iii+1)=-1.;}
  AB2.finalize();
  
  sizeAmat=498;
  SparseMatrix<double,1,int> AB3(sizeAmat,sizeAmat);
  AB3.reserve(3*sizeAmat);
  for (unsigned int iii=0;iii<sizeAmat;++iii){AB3.insert(iii,iii)=2.01;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB3.insert(iii+1,iii)=-1.;}
  for (unsigned int iii=0;iii<sizeAmat-1;++iii){AB3.insert(iii,iii+1)=-1.;}
  AB3.finalize();
  

  
  VectorXd inVec=VectorXd::Ones(sizeAmat*(sizeAmat+1)*(sizeAmat+2));
  VectorXd outVec=VectorXd::Zero(sizeAmat*(sizeAmat+1)*(sizeAmat+2));
  
  for(int iii=0;iii<sizeAmat*(sizeAmat+1)*(sizeAmat+2);++iii)
  {
    inVec[iii]=iii+1;
  }
  
  std::vector< SparseMatrix<double,1,int>, aligned_allocator<SparseMatrix<double,1,int> > > Qs;
  Qs.push_back(AB);
  Qs.push_back(AB2);
  Qs.push_back(AB3);
  
  // std::cout << Qs[0] << "\n\n" << Qs[1]  << "\n\n Length of vector: " << Qs.size() << "\n\n" << std::endl;
  
  timer matvectimer;
  matvectimer.start();
  krylstat_misc::kronMatVec(Qs,inVec,outVec);
  matvectimer.stop();
  std::cout << matvectimer << std::endl;
  
  matvectimer.restart();
  krylstat_misc::kronMatVec2(Qs,inVec,outVec);
  matvectimer.stop();
  std::cout << matvectimer << std::endl;
  
  // std::cout << outVec << std::endl;
  
  // std::cout << "\n\nSice: " << outVec.rows() << std::endl;
 
  
  return 0;
}