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
                                                                        
// $Revision: 1.2 $
// $Date: 2003/10/27 23:45:44 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/stepSize/ArmijoStepSizeRule.h,v $


//
// Written by Quan Gu (qgu@ucsd.edu)
//

#include "MonteCarloResponseAnalysis.h"
#include "ProbabilityTransformation.h"
#include "NatafProbabilityTransformation.h"
#include "RandomNumberGenerator.h"
#include "RandomVariable.h"
#include "RandomVariablePositioner.h"
#include "GFunEvaluator.h"
#include "NormalRV.h"
#include "Vector.h"
#include "Matrix.h"
#include "NormalRV.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include<ShellAPI.h>    // ShellExecuteEx
using namespace std;

using std::ifstream;
//using std::ios;
MonteCarloResponseAnalysis::MonteCarloResponseAnalysis(
	                    GFunEvaluator *passedGFunEvaluator,
						ReliabilityDomain *passedReliabilityDomain,
						Tcl_Interp *passedTclInterp,
						ProbabilityTransformation *passedProbabilityTransformation,
						RandomNumberGenerator *passedRandomNumberGenerator,
						int passedNumberOfSimulations,
						int passedPrintFlag,
						TCL_Char *passedFileName,
					//	TCL_Char *pTclFileToRunFileName,
						int pSeed
						)
{
	theGFunEvaluator = passedGFunEvaluator;
	theReliabilityDomain = passedReliabilityDomain;
	theTclInterp = passedTclInterp;
	theProbabilityTransformation = passedProbabilityTransformation;
	theRandomNumberGenerator = passedRandomNumberGenerator;
	numberOfSimulations = passedNumberOfSimulations;
	printFlag = passedPrintFlag;
	strcpy(fileName,passedFileName);
	seed = pSeed;
/*
	if (pTclFileToRunFileName !=0){
		tclFileToRun=new char [30];
		strcpy(tclFileToRun,pTclFileToRunFileName);
	}
	else tclFileToRun = 0;
*/
}



MonteCarloResponseAnalysis::~MonteCarloResponseAnalysis()
{
	//if (tclFileToRun !=0) delete [] tclFileToRun;
}

int MonteCarloResponseAnalysis::analyze() {


	cout << "Monte Carlo Response Analysis is running ... " << endl;
	double gFunValue = 0, gFunValue2 = 0, /*phi=0,*/ q = 0, q_bar = 0,/*h=0,*/ variance_of_q_bar = 0, cov_of_q_bar = 0;
	int result;
	int kk = 0;
	bool isFirstSimulation = true;
	int I = 0;
	bool failureHasOccured = false;
	//int seed = 1;
	if (printFlag == 2) {

		// check whether the restart file '_restart.tmp' exist, (this file is wrote by openSees only, not by user)
		//                      if yes, read data;{ success reading: set values above; otherwise: do noting} 
		//		                 if no, create file '_restart.tmp'
		//	data format:                 seed           numOfGFunEvaluations

		ifstream inputFile("_restart.tmp", ios::in);
		if (!inputFile) {
			// file doesn't exist; so it's safe to create it and write to it

		}
		else {
			inputFile >> seed;
			inputFile >> kk;
			inputFile.close();
			isFirstSimulation = false;
		}

	}

	int numRV = theReliabilityDomain->getNumberOfRandomVariables();

	Vector x(numRV);

	Vector u(numRV);
	Vector randomArray(numRV);

	NormalRV *aStdNormRV = 0;
	aStdNormRV = new NormalRV(1, 0.0, 1.0, 0.0);
	ofstream *outputFile = 0;



	// Check if computer ran out of memory
	if (aStdNormRV == 0) {
		cout << "MonteCarloResponseAnalysis::analyze() - out of memory while instantiating internal objects." << endl;
		return -1;
	}


	// Prepare output file
	ofstream resultsOutputFile(fileName, ios::out);

	char seps[] = ".";
	char * token;
	token = strtok(fileName, seps);
	strcpy(fileName, token);
	token = strtok(NULL, seps);
	char extention[5] = ".";
	if (token != NULL)
		strcat(extention, token);
	else
		strcat(extention, "out");
	char theStr[3] = "_U";
	strcat(fileName, theStr);
	strcat(fileName, extention);
	ofstream resultsOutputFile_1(fileName, ios::out);



	//////////////////
	ofstream tt6("G1G2.out");
	tt6.precision(16);
	tt6 << "k   G1     G2     Pf     cov " << endl;
	while (kk < numberOfSimulations) { // && govCov>targetCOV || k<=2) ) {

		ofstream resultsOutputFile_wind("Wind_Random_Value.dat");
		// Keep the user posted
		//if (printFlag == 1 || printFlag == 2) {
			//		if (kk%100 ==0)
		//	cout << "Sample #" << kk << ":" << endl;
			//			resultsOutputFile<< "Sample #" << kk << ":" << endl;

		//}
		resultsOutputFile_wind << kk << endl;


		// Create array of standard normal random numbers
		if (isFirstSimulation) {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV, seed);
		}
		else {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV);
		}
		seed = theRandomNumberGenerator->getSeed();
		if (result < 0) {
			cout << "MonteCarloResponseAnalysis::analyze() - could not generate" << endl
				<< " random numbers for simulation." << endl;
			return -1;
		}
		randomArray = theRandomNumberGenerator->getGeneratedNumbers();


		// Compute the point in standard normal space


		u = randomArray;   // Quan

		// Transform into original space
		result = theProbabilityTransformation->set_u(u);
		if (result < 0) {
			cout << "MonteCarloResponseAnalysis::analyze() - could not " << endl
				<< " set the u-vector for xu-transformation. " << endl;
			return -1;
		}


		result = theProbabilityTransformation->transform_u_to_x();
		if (result < 0) {
			cout << "MonteCarloResponseAnalysis::analyze() - could not " << endl
				<< " transform u to x. " << endl;
			return -1;
		}
		x = theProbabilityTransformation->get_x();


		// ------ here recorder x ----
  //		cout << "RV x is: " << x << endl;
  //		resultsOutputFile << "RV x is: " <<endl;
		resultsOutputFile.precision(15);
		for (int i = 0; i < numRV; i++) {
			resultsOutputFile << x(i) << endl;
			resultsOutputFile_1 << u(i) << endl;
			resultsOutputFile_wind << x(i) << endl;


		}
		//		resultsOutputFile <<endl;
		resultsOutputFile_wind.close();

		//compute structural response and gfunction
		//static GFunEvaluator *theGFunEvaluator;
		result = theGFunEvaluator->runGFunAnalysis(x);
		result = theGFunEvaluator->evaluateG(x);
		gFunValue = theGFunEvaluator->getG();

		//calculate the number of failure samples
		if (gFunValue <= 0.0) {
			I = I + 1;
			failureHasOccured = true;
		}

		tt6 << kk << "  " << gFunValue << "   " << gFunValue2 << "  ";
		resultsOutputFile << "k:" << kk << ",  " << "gFunValue:" << gFunValue << endl;
		cout << "Sample #" << kk << ",  " << "gFunValue:" << gFunValue << endl;

		kk++;
		isFirstSimulation = false;

		if (printFlag == 2) {
			// write necessary data into file '_restart.tmp' .... close file
			ofstream resultsOutputFile5("_restart.tmp");
			resultsOutputFile5 << seed << endl;
			resultsOutputFile5 << kk << endl;
			resultsOutputFile5.flush();
			resultsOutputFile5.close();
		}

	};// while 

	// Print summary of results to screen 
	cout << "Simulation Analysis completed." << endl;
	//Evaluate the failure probability
	double Pf;
	Pf = (double)I / numberOfSimulations;
	cout << "Pf = " << Pf << endl;
	double cov; //Represents the coefficient of variation
	cov = sqrt((1 - Pf) / (numberOfSimulations*Pf));
	cout << "c.o.v = " << cov << endl;
	// Clean up
	resultsOutputFile.close();
	resultsOutputFile_1.close();
	// resultsOutputFile_wind.close();
	// delete theMatrixOperations;
	delete aStdNormRV;
	return 0;
};