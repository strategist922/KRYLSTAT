#include <sampling/gmrfSampling.h>
#include <cusp/print.h>

typedef double ValueType;
typedef int IntType;

int main()
{
  int Nrat=5;
  int nSamp=10;
  ValueType minEig=1.e-2;
  ValueType maxEig=1.e2;
  double *intConst=new double;;
  double *wsq=new double[Nrat];
  double *dzdt=new double[Nrat];
  

  
  sqrtIntPoints(Nrat,minEig,maxEig,intConst,wsq,dzdt);
  
  
  cusp::array1d<ValueType,hostMem> wsqCusp(Nrat),dzdtCusp(Nrat),intConstCusp(1);
  arrayToCuspArray(Nrat,intConst,wsq,dzdt,wsqCusp,dzdtCusp,intConstCusp);
  delete[] wsq,dzdt;
  delete intConst;
  
  /*for (int iii=0;iii<Nrat;iii++)
  {
    std::cout << wsqCusp[iii] << "\n";
  }*/
  cusp::print(wsqCusp);
  //std::cout << wsqCusp << "\n";
  // create an empty sparse matrix structure (HYB format)
  cusp::hyb_matrix<int, ValueType, hostMem> A;

  
  // create a 2d Poisson problem on a 10x10 mesh
  cusp::gallery::poisson5pt(A, 10, 10);
  
  
  
    
  return 1;
}