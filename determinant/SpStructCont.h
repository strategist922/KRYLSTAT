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


/*#include <iostream.h> */
#include <string.h> 
/*#include <ColPackHeaders.h>*/

#ifndef SPSTRUCT_H
#define SPSTRUCT_H


template <typename IntegerType> 
class SpStructCont
{
  public:
    IntegerType num_rows;
    int **pptr;
    IntegerType nnz;
    // ColPack::GraphColoringInterface *graph;
    
    
    SpStructCont(IntegerType *row_offsets,IntegerType *column_indices,IntegerType nr) : num_rows(nr), pptr(new int*[(int)nr]),nnz(row_offsets[nr])
    {
      //IntegerType **pptr;
      /*this->num_rows=num_rows;
      pptr=new IntegerType*[num_rows];*/
      
      IntegerType new_ind=0;
      IntegerType cur_ind=0;
      IntegerType lengthRow=0;
      
      /*friend std::ostream& operator<< (std::ostream &out, SpStructCont<IntegerType> &SpS);*/

  
      IntegerType tempInd=0;
      for(IntegerType iii=0;iii<num_rows;iii++)
      {
	cur_ind=row_offsets[iii];
	//std::cout<<cur_ind << "\n";
	new_ind=row_offsets[iii+1];
	lengthRow=(new_ind-cur_ind);
	
	//std::cout << "We have come to first alloc \n\n\n";
	
	pptr[iii]=new int[lengthRow+1];
	pptr[iii][0]=lengthRow;
    
	tempInd=cur_ind;
	for(IntegerType jjj=1;jjj<=lengthRow;jjj++)
	{
	  pptr[iii][jjj]=column_indices[tempInd];
	  tempInd++;
	}
	//std::cout << pptr[iii][0] << "\n";
      }
      
    }
    
    
    
    ~SpStructCont()
    {
      for(int iii=0;iii<num_rows;iii++)
      {
	delete [] pptr[iii];    
      }
      delete [] pptr;
      pptr=0;
    }
    
    
    
    
    void printSPS()
    {
      IntegerType nonzs=0;
      for(IntegerType iii=0;iii<this->num_rows;++iii)
      {
	std::cout << "Row number " << iii << ". Number of nonzs: " << this->pptr[iii][0] << ". Columns: ";
	nonzs=this->pptr[iii][0];
	for(IntegerType jjj=1;jjj<=nonzs;++jjj)
	{
	  std::cout << this->pptr[iii][jjj];
	  if (jjj<nonzs)
	  {
	    std::cout << ", ";
	  }
	}
	std::cout << "\n";
      }
    }
    
    
};





#endif
