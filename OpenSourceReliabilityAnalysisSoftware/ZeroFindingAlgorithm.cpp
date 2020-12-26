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
**   Quan Gu (qgu@ucsd.edu)                                           **
**   Joel P. Conte (jpconte@ucsd.edu)                                 **
** ****************************************************************** */
                                                                        
 
//
// Written by  Quan Gu UCSD
//
 

#include "ZeroFindingAlgorithm.h"
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZeroFindingAlgorithm::ZeroFindingAlgorithm()
{
	theSamplingAnalysis=0;
	functionTol= 1.e-14;
	variableTol=1.e-7;

	a=0;
	b=0;
	zeroPoint=0;
	functionType=-1;
	iterationNum =0;
	maxIterNum = 20;


	ii_1=0;
	ii_2=0;
	x_1=0;
	x_2=0;
	G_1=0;
	G_2=0;
}

ZeroFindingAlgorithm::~ZeroFindingAlgorithm()
{
	if (x_1 != 0) { delete x_1; x_1=0;}
 	if (G_1 !=0 ) { delete G_1; G_1=0;}
	if (x_2 !=0 ) { delete x_2; x_2=0;}
	if (G_2 !=0 ) { delete G_2; G_2=0;}
}



void ZeroFindingAlgorithm::setA(double A){
	a=A;
};

void ZeroFindingAlgorithm::setB(double B){
	b=B;
};

double ZeroFindingAlgorithm::getZeroPoint()
{
	return zeroPoint;
}



	

int ZeroFindingAlgorithm::getIterationNumber()
{
	return iterationNum;
}

void ZeroFindingAlgorithm::setMaxIterNum(int newMaxIterNum)
{
	maxIterNum = newMaxIterNum;
}

void ZeroFindingAlgorithm::setFa(double pFa)
{
	if (pFa <0) { cout<<"Fatal:ZeroFindingAlgorithm::setFa() Fa<0"; };
	Fa=pFa;
}

void ZeroFindingAlgorithm::setFb(double pFb)
{
	if (pFb >0) { cout<<"Fatal:ZeroFindingAlgorithm::setFb() Fb>0"; };
	Fb=pFb;
}

double ZeroFindingAlgorithm::getFa()
{
	return Fa;
}

double ZeroFindingAlgorithm::getFb()
{
	return Fb;
}

double ZeroFindingAlgorithm::getA()
{
	return a;
}

double ZeroFindingAlgorithm::getB()
{
	return b;
}

void ZeroFindingAlgorithm::setFEconvergence(bool a)
{
	FEconvergence = a;
}


bool ZeroFindingAlgorithm::getFEconvergence(void){
	return FEconvergence;
}

void ZeroFindingAlgorithm::saveXG1(double pX1, double pG1)
{
	if ((x_1==0)||(G_1==0)) {
		cout<<"ZeroFindingAlgorithm::saveXG1 wrong"<<endl;
		if (x_1==0) x_1 = new double[100];
		if (G_1==0) G_1 = new double[100];
		ii_1=0;
	}
	
	x_1[ii_1]=pX1;
	G_1[ii_1]=pG1;
	ii_1++;

	if (ii_1>=100) {cout<< "Fatal: ZeroFindingAlgorithm::saveXG1() ii_1 larger than 100! \n"; return;}
	
	

}

void ZeroFindingAlgorithm::saveXG2(double pX2, double pG2)
{
	if ((x_2==0)||(G_2==0)) {
		cout<<"ZeroFindingAlgorithm::saveXG2 check whether re-alocate memory"<<endl;
		if (x_2==0) x_2 = new double[100];
		if (G_2==0) G_2 = new double[100];
		ii_2=0;
		
	 
	}
	
	x_2[ii_2]=pX2;
	G_2[ii_2]=pG2;
	ii_2++;

	if (ii_2>=100) {cout<< "Fatal: ZeroFindingAlgorithm::saveXG2() ii_2 larger than 100! \n"; return;}
	
	
}

double ZeroFindingAlgorithm::getX2(int i)
{
	if(i>=ii_2) { cout<<"wrong in ZeroFindingAlgorithm::getX2.  i>=ii_2 "<<endl;return -999;};
	return x_2[i];
}

double ZeroFindingAlgorithm::getG2(int i)
{
	if(i>=ii_2) { cout<<"wrong in ZeroFindingAlgorithm::getG2.  i>=ii_2 "<<endl; return -999;};
	return G_2[i];
}

double ZeroFindingAlgorithm::getX1(int i)
{
	if(i>=ii_1) { cout<<"wrong in ZeroFindingAlgorithm::getX1.  i>=ii_1 "<<endl;return -999;};
	return x_1[i];
}

double ZeroFindingAlgorithm::getG1(int i)
{
	if(i>=ii_1) { cout<<"wrong in ZeroFindingAlgorithm::getG1.  i>=ii_1 "<<endl; return -999;};
	return G_1[i];
}
void ZeroFindingAlgorithm::setX1Pointer(double * pX1)
{
	cout<<"warning: ZeroFindingAlgorithm::setX1Pointer()  should not be called. It may cause lose of memory.\n";
	this->x_1 = pX1;
}

void ZeroFindingAlgorithm::setG1Pointer(double * pG1)
{
	cout<<"warning: ZeroFindingAlgorithm::setG1Pointer()  should not be called. It may cause lose of memory.\n";
	this->G_1 = pG1;
}


void ZeroFindingAlgorithm::setX2Pointer(double * pX2)
{
	cout<<"warning: ZeroFindingAlgorithm::setX2Pointer()  should not be called. It may cause lose of memory.\n";
	this->x_2 = pX2;
}

void ZeroFindingAlgorithm::setG2Pointer(double * pG2)
{
	cout<<"warning: ZeroFindingAlgorithm::setG2Pointer()  should not be called. It may cause lose of memory.\n";
	this->G_2 = pG2;
}

void ZeroFindingAlgorithm::set_ii_1(int pI)
{
	this->ii_1=pI;
}

void ZeroFindingAlgorithm::set_ii_2(int pI)
{
	this->ii_2=pI;
}

int ZeroFindingAlgorithm::get_ii_1()
{
	return ii_1;
}

int ZeroFindingAlgorithm::get_ii_2()
{
	return ii_2;
}

void ZeroFindingAlgorithm::setFunctionType(int i)
{
	this->functionType = i;
}

int ZeroFindingAlgorithm::getFunctionType()
{
	return this->functionType;
}

//	int functionType;   =1: samplingAnalysis. first surface  =2: upcrossing. second surface

double * ZeroFindingAlgorithm::getStoredSmallestPositive(){

	static double tmp[2];

	tmp[0]=0; tmp[1]=-1.0e11;

	if (functionType == 1){
		if (ii_1 ==0) { // do nothing, no stored value
		}
		else {
			double positiveValue = 1.0e11; 

			for (int i =0; i<ii_1; i++){
				if(getG1(i)>=0 && getG1(i)<positiveValue){
				   positiveValue = getG1(i);
				   tmp[0] = getX1(i);
				   tmp[1] = positiveValue;				
				} //if
							
			}//for

			if (positiveValue == 1.0e11){  // no value found
			   	tmp[0]=0; 
				tmp[1]=-1.0e11;
			} //if
		
		}//else

	} //if {functionType == 1}
	else if(functionType == 2){
	
		if (ii_2 ==0) { // do nothing, no stored value
		}
		else {
			double positiveValue = 1.0e11; 

			for (int i =0; i<ii_2; i++){
				if(getG2(i)>=0 && getG2(i)<positiveValue){
				   positiveValue = getG2(i);
				   tmp[0] = getX2(i);
				   tmp[1] = positiveValue;
				
				} //if
							
			}//for

			if (positiveValue == 1.0e11){  // no value found
			   	tmp[0]=0; 
				tmp[1]=-1.0e11;
			} //if
		
		}//else

	} //if {functionType == 2}
	


	return tmp;

};

double * ZeroFindingAlgorithm::getStoredLargestNegative(){

	static double tmp[2];

	tmp[0]=0; tmp[1]= 1.0e11;

	if (functionType == 1){
		if (ii_1 ==0) { // do nothing, no stored value
		}
		else {
			double NegativeValue = -1.0e11; 

			for (int i =0; i<ii_1; i++){
				if(getG1(i)<=0 && getG1(i)>NegativeValue){
				   NegativeValue = getG1(i);
				   tmp[0] = getX1(i);
				   tmp[1] = NegativeValue;				
				} //if
							
			}//for

			if (NegativeValue == -1.0e11){  // no value found
			   	tmp[0]=0; 
				tmp[1]=1.0e11;
			} //if
		
		}//else

	} //if {functionType == 1}
	else if(functionType == 2){
		if (ii_2 ==0) { // do nothing, no stored value
		}
		else {
			double NegativeValue = -1.0e11; 

			for (int i =0; i<ii_2; i++){
				if(getG2(i)<=0 && getG2(i)>NegativeValue){
				   NegativeValue = getG2(i);
				   tmp[0] = getX2(i);
				   tmp[1] = NegativeValue;				
				} //if
							
			}//for

			if (NegativeValue == -1.0e11){  // no value found
			   	tmp[0]=0; 
				tmp[1]=1.0e11;
			} //if
		
		}//else

	} //if {functionType == 2}

	return tmp;


};
