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
                                                                        
// $Revision: 1.4 $
// $Date: 2003/10/27 23:45:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/gFunction/GFunEvaluator.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "GFunEvaluator.h"
#include "ReliabilityDomain.h"
#include <tcl.h>

#include <fstream>
#include <iomanip>
#include <iostream>
using std::ifstream;
using std::ios;
using std::setw;
using std::setprecision;
using namespace std;

GFunEvaluator::GFunEvaluator(Tcl_Interp *passedTclInterp, ReliabilityDomain *passedReliabilityDomain)
{
	theTclInterp = passedTclInterp;
	theReliabilityDomain = passedReliabilityDomain;
	numberOfEvaluations = 0;
}

GFunEvaluator::~GFunEvaluator()
{
}


double
GFunEvaluator::getG()
{
	return g;
}

int
GFunEvaluator::initializeNumberOfEvaluations()
{
	numberOfEvaluations = 0;
	return 0;
}

int
GFunEvaluator::getNumberOfEvaluations()
{
	return numberOfEvaluations;
}


int 
GFunEvaluator::evaluateG(Vector x)
{
	numberOfEvaluations++;


	// "Download" limit-state function from reliability domain
	int lsf = theReliabilityDomain->getTagOfActiveLimitStateFunction();
	LimitStateFunction *theLimitStateFunction = theReliabilityDomain->getLimitStateFunctionPtr(lsf);


	// Get the limit-state function expression
	char *theExpression = theLimitStateFunction->getExpression();


	// Set value of GFun-specific parameters in the limit-state function
	int result = this->tokenizeSpecials(theExpression);


	// Initial declarations
	int i;
	double fileValue = 0.0;

	char buf[1500]="";
	char tclAssignment[1500]="";
	char tempchar[1500]="";
	char temp[1500];

	char separators[5] = "}{";
	char dollarSign[500] = "$";
	char underscore[500] = "_";

	char lsf_forTokenizing[1500];
	strcpy(lsf_forTokenizing,theExpression);


	// Set values of basic random variables and file quantities
	// (Other quantities will have to be set by specific implementations of this class)
	char *tokenPtr = strtok( lsf_forTokenizing, separators);
	while ( tokenPtr != NULL ) {

		// Copy the token pointer over to a temporary storage
		strcpy(tempchar,tokenPtr);

		if ( strncmp(tokenPtr, "x",1) == 0) {
			int rvNum;
			sscanf(tempchar,"x_%i",&rvNum);
			sprintf(tclAssignment , "set x_%d  %15.5f", rvNum, x(rvNum-1) );
			Tcl_Eval( theTclInterp, tclAssignment);
		}
		else if ( strncmp(tokenPtr, "file",4) == 0) {
			int rowNum = 0;
			int colNum = 0;
			char fileName[256];
			sscanf(tempchar,"file_%s",fileName);
			int rowloc = strcspn(fileName,"_");
			char rowstr[10] = "";
			int rowcnt = 0;
			for (i=rowloc+1; fileName[i]!='\0'; i++) {
				rowstr[rowcnt] = fileName[i];
				rowcnt++;
			}
			rowstr[rowcnt] = '\0';
			sscanf(rowstr,"%i_%i",&rowNum,&colNum);
			fileName[rowloc] = '\0';

			ifstream inputFile(fileName,ios::in);
			if (!inputFile) {
				cout << "Could not open file with quantities for limit-state function." << endl;
			}
			for (i=1; i<rowNum; i++) {
				inputFile.getline(buf,120);
			}
			for (i=1; i<=colNum; i++) {
				inputFile >> temp;
			}
			fileValue = (double)atof(temp);
			if (fileValue == 0.0) {
				cout << "ERROR: Could not find quantity in performance function file." << endl;
				return -1;
			}
			inputFile.close();
			sprintf(tclAssignment , "set file_%s_%d_%d  %15.5f",fileName,rowNum,colNum,fileValue);

			Tcl_Eval( theTclInterp, tclAssignment);
		}
		
		tokenPtr = strtok( NULL, separators);
	}

	// Compute value of g-function
	char *theTokenizedExpression = theLimitStateFunction->getTokenizedExpression();
	g = 0.0;
	Tcl_ExprDouble( theTclInterp, theTokenizedExpression, &g );
	//通过TCL将表达式theTokenizedExpression的值进行计算并赋值给g


	return 0;
}






void
GFunEvaluator::setNsteps(int nsteps)
{
	cout << "GFunEvaluator::set_nsteps() -- This method is not " << endl
		<< " implemented for the chosen type of GFunEvaluator." << endl;
}


double
GFunEvaluator::getDt()
{
	cout << "GFunEvaluator::getDt() -- This method is not " << endl
		<< " implemented for the chosen type of GFunEvaluator." << endl;
	return 0;
}







double GFunEvaluator::getG2(double g, double littleDt)   //Quan
{
	cout<<"Fatal::GFunEvaluator::getG2() not implemented yet!"<<endl; exit(-1);

	return 0.0;
}
