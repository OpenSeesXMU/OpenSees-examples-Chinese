/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2001, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** Reliability module developed by:                                   **
**   Terje Haukaas (haukaas@ce.berkeley.edu)                          **
**   Armen Der Kiureghian (adk@ce.berkeley.edu)                       **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.1 $
// $Date: 2003/10/27 23:04:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/filter/KooFilter.h,v $


//  Written by Quan and Michele  Dec. 2005 

#include "DeltaFilter.h"
#include "Filter.h"
#include "classTags.h"
#include <iostream>
using namespace std;
#define FILTER_DELTA  9877101

DeltaFilter::DeltaFilter(int tag)
:Filter(tag,FILTER_DELTA)
{

}

DeltaFilter::~DeltaFilter()
{
}

double
DeltaFilter::getAmplitude(double time,double dT)
{
  if (fabs(time)>dT+1.0e-14) return 0.0;
  else return 1.0-fabs(time)/dT;
	
}

double
DeltaFilter::getMaxAmplitude()
{

		return  1.0;

}

double 
DeltaFilter::getTimeOfMaxAmplitude(){ 
	cout<<"warning: DeltaFilter::getTimeOfMaxAmplitude is called!"<<endl;
	return 0.0;
};


void
DeltaFilter::Print(OPS_Stream &s, int flag)  
{
}

