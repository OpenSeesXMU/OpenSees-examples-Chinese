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
// $Date: 2005/06/16 21:57:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/analysis/ImportanceSamplingAnalysis.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "ImportanceSamplingAnalysis.h"
#include "ReliabilityDomain.h"
#include "ReliabilityAnalysis.h"
#include "LimitStateFunction.h"
#include "ProbabilityTransformation.h"
#include "NatafProbabilityTransformation.h"
#include "GFunEvaluator.h"
#include "BasicGFunEvaluator.h"
#include "RandomNumberGenerator.h"
#include "RandomVariable.h"
#include "NormalRV.h"
#include "Vector.h"
#include "Matrix.h"
#include "MatrixOperations.h"
#include "NormalRV.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
using std::ifstream;
using std::ios;
using std::setw;
using std::setprecision;
using std::setiosflags;
using namespace std;



ImportanceSamplingAnalysis::ImportanceSamplingAnalysis(	Tcl_Interp *pInterp,
								       ReliabilityDomain *passedReliabilityDomain,
										ProbabilityTransformation *passedProbabilityTransformation,
										GFunEvaluator *passedGFunEvaluator,
										RandomNumberGenerator *passedRandomNumberGenerator,
										int passedNumberOfSimulations,
										double passedTargetCOV,
										double passedSamplingStdv,
										int passedPrintFlag,
										TCL_Char *passedFileName,
										Vector *pStartPoint,
										int passedAnalysisTypeTag)
:ReliabilityAnalysis()
{
	theReliabilityDomain = passedReliabilityDomain;
	theProbabilityTransformation = passedProbabilityTransformation;
	theGFunEvaluator = passedGFunEvaluator;
	theRandomNumberGenerator = passedRandomNumberGenerator;
	numberOfSimulations = passedNumberOfSimulations;
	targetCOV = passedTargetCOV;
	samplingStdv = passedSamplingStdv;
	printFlag = passedPrintFlag;
	fileName = new char[50];
	strcpy(fileName,passedFileName);
	startPoint = pStartPoint;
	analysisTypeTag = passedAnalysisTypeTag;
	theInterp = pInterp;
	
}




ImportanceSamplingAnalysis::~ImportanceSamplingAnalysis()
{
	if (fileName != 0)
		delete [] fileName;
}



int 
ImportanceSamplingAnalysis::analyze(void)
{

	// Alert the user that the simulation analysis has started
	cout << "Sampling Analysis is running ... " << endl;

	// Declaration of some of the data used in the algorithm
	double gFunctionValue;
	int result;
	int I, i, j;
	int k = 1;
	int seed = 1;
	double det_covariance;
	double phi;
	double h;
	double q;
	int numRV = theReliabilityDomain->getNumberOfRandomVariables();
	Matrix covariance(numRV, numRV);
	Matrix chol_covariance(numRV, numRV);
	Matrix inv_covariance(numRV, numRV);
	Vector startValues(numRV);
	Vector x(numRV);
	Vector z(numRV);
	Vector u(numRV);
	Vector randomArray(numRV);
	LimitStateFunction *theLimitStateFunction = 0;
	NormalRV *aStdNormRV = 0;
	aStdNormRV = new NormalRV(1,0.0,1.0,0.0);
//	ofstream *outputFile = 0;
	bool failureHasOccured = false;
	char myString[50];

	
	// Check if computer ran out of memory
	if (aStdNormRV==0) {
		cout << "ImportanceSamplingAnalysis::analyze() - out of memory while instantiating internal objects." << endl;
		return -1;
	}

	
	// Establish covariance matrix
	for (i=0;  i<numRV;  i++) {
		for (j=0;  j<numRV;  j++) {
			if (i==j) {
				covariance(i,j) = samplingStdv*samplingStdv;
			}
			else
				covariance(i,j) = 0.0;
		}
	}


	// Create object to do matrix operations on the covariance matrix
	MatrixOperations *theMatrixOperations = 0;
	theMatrixOperations = new MatrixOperations(covariance);
	if (theMatrixOperations == 0) {
		cout << "ImportanceSamplingAnalysis::analyze() - could not create" << endl
			<< " the object to perform matrix operations." << endl;
		return -1;
	}


	// Cholesky decomposition of covariance matrix
	result = theMatrixOperations->computeLowerCholesky();
	if (result < 0) {
		cout << "ImportanceSamplingAnalysis::analyze() - could not compute" << endl
			<< " the Cholesky decomposition of the covariance matrix." << endl;
		return -1;
	}
	chol_covariance = theMatrixOperations->getLowerCholesky();


	// Inverse of covariance matrix
	result = theMatrixOperations->computeInverse();
	if (result < 0) {
		cout << "ImportanceSamplingAnalysis::analyze() - could not compute" << endl
			<< " the inverse of the covariance matrix." << endl;
		return -1;
	}
	inv_covariance = theMatrixOperations->getInverse();


	// Compute the determinant, knowing that this is a diagonal matrix
	result = theMatrixOperations->computeTrace();
	if (result < 0) {
		cout << "ImportanceSamplingAnalysis::analyze() - could not compute" << endl
			<< " the trace of the covariance matrix." << endl;
		return -1;
	}
	det_covariance = theMatrixOperations->getTrace();
	

	// Pre-compute some factors to minimize computations inside simulation loop
	double pi = 3.14159265358979;
	double factor1 = 1.0 / ( pow((2.0*pi),((double)numRV/2.0)) );
	double factor2 = 1.0 / ( pow((2.0*pi),((double)numRV/2.0)) * sqrt(det_covariance) );


	// Number of limit-state functions
	int numLsf = theReliabilityDomain->getNumberOfLimitStateFunctions();


	Vector sum_q(numLsf);
	Vector sum_q_squared(numLsf);
	double var_qbar;
	double pfIn;
	double CovIn;
 
	//sprintf(restartFileName,"%s_%s","restart",fileName);

	if (analysisTypeTag == 1) {
		// Possible read data from file if this is a restart simulation
		if (printFlag == 2) {
			ifstream inputFile( "_restartFile_.out", ios::in );
			inputFile >> k;
			inputFile >> seed;
			if (k==1 && seed==1) {
			}
			else { 
				for (int i=1; i<=numLsf; i++) {
					inputFile >> pfIn;
					inputFile >> CovIn;
					sum_q(i-1) = pfIn*k;
					var_qbar = (CovIn*pfIn)*(CovIn*pfIn);
					if (k<1.0e-6) {
						cout << "WARNING: Zero number of samples read from restart file" << endl;
					}
					sum_q_squared(i-1) = k*( k*var_qbar + pow(sum_q(i-1)/k,2.0) );
				}
				k++;
			}
			inputFile.close();
		}
	}


	// Transform start point into standard normal space, 
	// unless it is the origin that is to be sampled around
	Vector startPointY(numRV);
	if (startPoint == 0) {
		// Do nothing; keep it as zero
	}
	else {
		result = theProbabilityTransformation->set_x(*startPoint);
		if (result < 0) {
			cout << "ImportanceSamplingAnalysis::analyze() - could not " << endl
				<< " set the x-vector for xu-transformation. " << endl;
			return -1;
		}
		result = theProbabilityTransformation->transform_x_to_u();
		if (result < 0) {
			cout << "ImportanceSamplingAnalysis::analyze() - could not " << endl
				<< " transform x to u. " << endl;
			return -1;
		}
		startPointY = theProbabilityTransformation->get_u();
	}

	// ============= fix the problem for FE divergence while phi/h too big ====
	double  beta  = startPointY.Norm();
	double criterion = 10.0 * exp( -0.5 * beta*beta);
	if (criterion < 1.0e-14) criterion = 1.0e20; // no criterion since startPointY does not exist.
   // ==================


	// Initial declarations
	Vector cov_of_q_bar(numLsf);
	Vector q_bar(numLsf);
	Vector variance_of_q_bar(numLsf);
	Vector responseVariance(numLsf);
	Vector responseStdv(numLsf);
	Vector g_storage(numLsf);
	Vector sum_of_g_minus_mean_squared(numLsf);
	Matrix crossSums(numLsf,numLsf);
	Matrix responseCorrelation(numLsf,numLsf);
	char string[60];
	Vector pf(numLsf);
	Vector cov(numLsf);
	double govCov = 999.0;
	Vector temp1;
	double temp2;
	double denumerator;
	bool FEconvergence;


	// Prepare output file
	ofstream resultsOutputFile( fileName, ios::out );
	ofstream resultsOutputFile_aa( "recorded_sampling_points_converged_.out", ios::out );
    ofstream resultsOutputFile_bb( "recorded_sampling_points_diverged_.out", ios::out );
	ofstream resultsOutputFile_cc( "recorded_sampling_failure_Pt_U.out", ios::out );
	ofstream resultsOutputFile_dd( "recorded_sampling_failure_Pt_X.out", ios::out );

	ofstream out("ignoredPoint.out", ios::out);
	
	bool isFirstSimulation = true;  //==========================================
	while( (k<=numberOfSimulations && govCov>targetCOV || k<=2) ) {

		// Keep the user posted
		if (printFlag == 1 || printFlag == 2) {
			cout << "Sample #" << k << ":" << endl;
		}

		
		// Create array of standard normal random numbers
		if (isFirstSimulation) {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV,seed);
		}
		else {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV);
		}
		seed = theRandomNumberGenerator->getSeed();
		if (result < 0) {
			cout << "ImportanceSamplingAnalysis::analyze() - could not generate" << endl
				<< " random numbers for simulation." << endl;
			return -1;
		}
		randomArray = theRandomNumberGenerator->getGeneratedNumbers();

		// Compute the point in standard normal space
		u = startPointY + chol_covariance * randomArray;
		
		//cout<<"startPointY: "<<startPointY<<endl;
		
		//cout<<"chol_covariance * randomArray:  "<<chol_covariance * randomArray<<endl;
                
		// Transform into original space
		result = theProbabilityTransformation->set_u(u);
		if (result < 0) {
			cout << "ImportanceSamplingAnalysis::analyze() - could not " << endl
				<< " set the u-vector for xu-transformation. " << endl;
			return -1;
		}

		
		result = theProbabilityTransformation->transform_u_to_x();
		if (result < 0) {
			cout << "ImportanceSamplingAnalysis::analyze() - could not " << endl
				<< " transform u to x. " << endl;
			return -1;
		}
		x = theProbabilityTransformation->get_x();

	/*	cout<<"In standard normal space u is: "<<endl;
		cout<<u<<endl;
		cout<<"In physical space x is: "<<endl;
		for (int i=0; i<x.Size();i++)	cout<<x(i)<<"  ";
		cout<<endl;
*/
					


		// Evaluate limit-state function
		FEconvergence = true;
		result = theGFunEvaluator->runGFunAnalysis(x);

     /* debug purpose --- information recovery----
		
		if (result>=0) {
			resultsOutputFile_aa.precision(16);
			resultsOutputFile_aa<<k<<"  \n  ";
 			for (int i=0; i<x.Size();i++)	resultsOutputFile_aa<<x(i)<<" \n   ";
			resultsOutputFile_aa.flush();
		}

		else if (result<0) {

		}

		// ================
	*/	
		
		if (result < 0) {
			// In this case a failure happened during the analysis
			// Hence, register this as failure
		
		 	FEconvergence = false;  
		}


		// Loop over number of limit-state functions
		for (int lsf=0; lsf<numLsf; lsf++ ) {


			// Set tag of "active" limit-state function
			theReliabilityDomain->setTagOfActiveLimitStateFunction(lsf+1);


			// Get value of limit-state function
			result = theGFunEvaluator->evaluateG(x);
			if (result < 0) {
				cout << "ImportanceSamplingAnalysis::analyze() - could not " << endl
					<< " tokenize limit-state function. " << endl;
				return -1;
			}
			gFunctionValue = theGFunEvaluator->getG();
			if (!FEconvergence) {
				gFunctionValue = -1.0;  // Quan 2006 Dec. For pushover case, the failure may not happen even if diverge ??

			}
			
				
	 

			// ESTIMATION OF FAILURE PROBABILITY
			if (analysisTypeTag == 1) {


 
 
// ================ end Quan 2010 sep. 17th ==============

				// Collect result of sampling
				if (gFunctionValue < 0.0) {
					I = 1;
					failureHasOccured = true;
					if (printFlag == 2){
						for ( int i =0; i< x.Size(); i++){
							resultsOutputFile_cc<<u(i)<<"  \n  ";
							resultsOutputFile_dd<<x(i)<<"  \n  ";
						}
					}
				}
				else {
					I = 0;
				}


				// Compute values of joint distributions at the u-point
				phi = factor1 * exp( -0.5 * (u ^ u) );
				temp1 = inv_covariance ^ (u-startPointY);
				temp2 = temp1 ^ (u-startPointY);
				h   = factor2 * exp( -0.5 * temp2 );


				// Update sums
				

				q = I * phi / h;



				if (!FEconvergence) {  // recorder point and skip this point

						resultsOutputFile_bb.precision(16);
						resultsOutputFile_bb<<q<<"  \n  ";

 						for (int i=0; i<x.Size();i++)	resultsOutputFile_bb<<x(i)<<"  \n  ";
						resultsOutputFile_bb.flush();

						cout<<"warning: bad point "<<k<<" is stored! valid for ONLY one limit state function! "<<endl; 
						k--; 

			
				} //if(!FEconvergence)
					
	

				else {
					sum_q(lsf) = sum_q(lsf) + q;
					sum_q_squared(lsf) = sum_q_squared(lsf) + q*q;



					if (sum_q(lsf) > 0.0) {
						// Compute coefficient of variation (of pf)
						q_bar(lsf) = 1.0/(double)k * sum_q(lsf);
						variance_of_q_bar(lsf) = 1.0/(double)k * 
							( 1.0/(double)k * sum_q_squared(lsf) - (sum_q(lsf)/(double)k)*(sum_q(lsf)/(double)k));
						if (variance_of_q_bar(lsf) < 0.0) {
							variance_of_q_bar(lsf) = 0.0;
						}
						cov_of_q_bar(lsf) = sqrt(variance_of_q_bar(lsf)) / q_bar(lsf);
					}//if
				}//else
			} //(analysisTypeTag == 1)
			else if (analysisTypeTag == 2) {
			// ESTIMATION OF RESPONSE STATISTICS

				// Now q=g and q_bar=mean
				q = gFunctionValue; 
				failureHasOccured = true;
				
				sum_q(lsf) = sum_q(lsf) + q;
				sum_q_squared(lsf) = sum_q_squared(lsf) + q*q;

				g_storage(lsf) = gFunctionValue;
				
				if (sum_q(lsf) > 0.0) {
					
					// Compute coefficient of variation (of mean)
					q_bar(lsf) = 1.0/(double)k * sum_q(lsf);
					variance_of_q_bar(lsf) = 1.0/(double)k * 
						( 1.0/(double)k * sum_q_squared(lsf) - (sum_q(lsf)/(double)k)*(sum_q(lsf)/(double)k));
					if (variance_of_q_bar(lsf) < 0.0) {
						variance_of_q_bar(lsf) = 0.0;
					}
					cov_of_q_bar(lsf) = sqrt(variance_of_q_bar(lsf)) / q_bar(lsf);

					// Compute variance and standard deviation
					if (k>1) {
						responseVariance(lsf) = 1.0/((double)k-1) * (  sum_q_squared(lsf) - 1.0/((double)k) * sum_q(lsf) * sum_q(lsf)  );
					}
					else {
						responseVariance(lsf) = 1.0;
					}

					if (responseVariance(lsf) <= 0.0) {
						cout << "ERROR: Response variance of limit-state function number "<< lsf << endl
							<< " is zero! " << endl;
					}
					else {
						responseStdv(lsf) = sqrt(responseVariance(lsf));
					}
				}
			}//(analysisTypeTag == 2)
			else if (analysisTypeTag == 3) {
				// Store g-function values to file (one in each column)
				sprintf(myString,"%12.6e",gFunctionValue);
				resultsOutputFile << myString << "  ";
				resultsOutputFile.flush();
			}

            //Quan and Michele Mar 2006 ------------------------
			else if (analysisTypeTag == 4) {

				// only if FEconvergence, the mean upcrossing Rate has meaning..
				if(FEconvergence){
					double littleDeltaT =  1.e-3;  //why this is not from tcl? Quan 2007 June

					double g2FunctionValue = theGFunEvaluator->getG2(gFunctionValue, littleDeltaT);




					// Collect result of sampling
					if ((gFunctionValue < 0.0) && (g2FunctionValue > 0.0)) {
						I = 1;
						failureHasOccured = true;
					}
					else {
						I = 0;
					}


					// Compute values of joint distributions at the u-point
					phi = factor1 * exp( -0.5 * (u ^ u) );
					temp1 = inv_covariance ^ (u-startPointY);
					temp2 = temp1 ^ (u-startPointY);
					h   = factor2 * exp( -0.5 * temp2 );


					// Update sums
					q = I * phi / h;
					sum_q(lsf) = sum_q(lsf) + q;
					sum_q_squared(lsf) = sum_q_squared(lsf) + q*q;



					if (sum_q(lsf) > 0.0) {
						// Compute coefficient of variation (of pf)
						q_bar(lsf) = 1.0/(double)k * sum_q(lsf);
						variance_of_q_bar(lsf) = 1.0/(double)k * 
							( 1.0/(double)k * sum_q_squared(lsf) - (sum_q(lsf)/(double)k)*(sum_q(lsf)/(double)k));
						if (variance_of_q_bar(lsf) < 0.0) {
							variance_of_q_bar(lsf) = 0.0;
						}
						cov_of_q_bar(lsf) = sqrt(variance_of_q_bar(lsf)) / q_bar(lsf);
					}

				} // if FEConvergence. 
				else{ // divergende
				  k--;
				// ignore this point
				}
			 cout.precision(16);
             cout<<"Prob:"<<q_bar(lsf)<<"        cov:"<<cov_of_q_bar(lsf)<<endl;
			} //else if (analysisTypeTag == 4)
			
// ------------------------------------------------

			else {
				cout << "ERROR: Invalid analysis type tag found in sampling analysis." << endl;
			}

			// Keep the user posted
			if ( (printFlag == 1 || printFlag == 2) && analysisTypeTag!=3 && analysisTypeTag!=4) {
				sprintf(string," GFun #%d, estimate:%15.10f, cov:%15.10f",lsf+1,q_bar(lsf),cov_of_q_bar(lsf));
				cout << string << endl;
			}
		}// -- end loop of limit state function

		// Now all the limit-state functions have been looped over


		if (analysisTypeTag == 3) {
			resultsOutputFile << endl;
		}


		// Possibly compute correlation coefficient
		if (analysisTypeTag == 2) {

			for (int i=0; i<numLsf; i++) {
				for (int j=i+1; j<numLsf; j++) {

					crossSums(i,j) = crossSums(i,j) + g_storage(i) * g_storage(j);

					denumerator = 	(sum_q_squared(i)-1.0/(double)k*sum_q(i)*sum_q(i))
									*(sum_q_squared(j)-1.0/(double)k*sum_q(j)*sum_q(j));

					if (denumerator <= 0.0) {
						responseCorrelation(i,j) = 0.0;
					}
					else {
						responseCorrelation(i,j) = (crossSums(i,j)-1.0/(double)k*sum_q(i)*sum_q(j))
							/(sqrt(denumerator));
					}
				}
			}
		}

		
		// Compute governing coefficient of variation
		if (!failureHasOccured) {
			govCov = 999.0;
		}
		else {
			govCov = 0.0;
			for (int mmmm=0; mmmm<numLsf; mmmm++) {
				if (cov_of_q_bar(mmmm) > govCov) {
					govCov = cov_of_q_bar(mmmm);
				}
			}
		}

		
		// Make sure the cov isn't exactly zero; that could be the case if only failures
		// occur in cases where the 'q' remains 1
		if (govCov == 0.0) {
			govCov = 999.0;
		}


		// Print to the restart file, if requested. 
	
		ofstream outputFile( "_restartFile_.out", ios::out );
		outputFile << k << endl;
		outputFile << seed << endl;
		for ( int lsf=0; lsf<numLsf; lsf++ ) {
			sprintf(string,"%15.10f \n  %15.10f",q_bar(lsf),cov_of_q_bar(lsf));
			outputFile << string << " " << endl;
		}
        outputFile.close();
        cout<<"Prob:"<<q_bar(0)<<"        cov:"<<cov_of_q_bar(0)<<endl;	

		// Increment k (the simulation number counter)
	//	if (FEconvergence ||(analysisTypeTag != 4) ){
	//	if (analysisTypeTag != 4 ){

		k++;
		isFirstSimulation = false;
				
 

	} // while( (k<=numberOfSimulations && govCov>targetCOV || k<=2) )

	// Step 'k' back a step now that we went out
	k--;
	cout << endl;


	if (analysisTypeTag != 3) {

		if (!failureHasOccured) {
			cout << "WARNING: Failure did not occur for any of the limit-state functions. " << endl;
		}

		
		for (int lsf=1; lsf<=numLsf; lsf++ ) {

			if ( q_bar(lsf-1) == 0.0 ) {

				resultsOutputFile << "#######################################################################" << endl;
				resultsOutputFile << "#  SAMPLING ANALYSIS RESULTS, LIMIT-STATEFUNCDTION NUMBER   "
					<<setiosflags(ios::left)<<setprecision(1)<<setw(4)<<lsf <<"      #" << endl;
				resultsOutputFile << "#                                                                     #" << endl;
				resultsOutputFile << "#  Failure did not occur, or zero response!                           #" << endl;
				resultsOutputFile << "#                                                                     #" << endl;
				resultsOutputFile << "#######################################################################" << endl << endl << endl;
			}
			else {

				// Some declarations
				double beta_sim, pf_sim, cov_sim;
				int num_sim;


				// Set tag of "active" limit-state function
				theReliabilityDomain->setTagOfActiveLimitStateFunction(lsf);


				// Get the limit-state function pointer
				theLimitStateFunction = 0;
				//lsf = theReliabilityDomain->getTagOfActiveLimitStateFunction();
				theLimitStateFunction = theReliabilityDomain->getLimitStateFunctionPtr(lsf);
				if (theLimitStateFunction == 0) {
					cout << "ImportanceSamplingAnalysis::analyze() - could not find" << endl
						<< " limit-state function with tag #" << lsf << "." << endl;
					return -1;
				}

			
				// Store results
				//(analysisTypeTag == 1) {
				if ((analysisTypeTag == 1) || (analysisTypeTag == 4)) {
					beta_sim = -aStdNormRV->getInverseCDFvalue(q_bar(lsf-1));
					pf_sim	 = q_bar(lsf-1);
					cov_sim	 = cov_of_q_bar(lsf-1);
					num_sim  = k;
					theLimitStateFunction->SimulationReliabilityIndexBeta = beta_sim;
					theLimitStateFunction->SimulationProbabilityOfFailure_pfsim = pf_sim;
					theLimitStateFunction->CoefficientOfVariationOfPfFromSimulation = cov_sim;
					theLimitStateFunction->NumberOfSimulations = num_sim;
				}


				// Print results to the output file
				//(analysisTypeTag == 1) {
				if ((analysisTypeTag == 1) || (analysisTypeTag == 4)) {
					resultsOutputFile << "#######################################################################" << endl;
					resultsOutputFile << "#  SAMPLING ANALYSIS RESULTS, LIMIT-STATE FUNCTION NUMBER   "
						<<setiosflags(ios::left)<<setprecision(1)<<setw(4)<<lsf <<"      #" << endl;
					resultsOutputFile << "#                                                                     #" << endl;
					resultsOutputFile << "#  Reliability index beta: ............................ " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<beta_sim 
						<< "  #" << endl;
					resultsOutputFile << "#  Estimated probability of failure pf_sim: ........... " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<pf_sim 
						<< "  #" << endl;
					resultsOutputFile << "#  Number of simulations: ............................. " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<num_sim 
						<< "  #" << endl;
					resultsOutputFile << "#  Coefficient of variation (of pf): .................. " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<cov_sim 
						<< "  #" << endl;
					resultsOutputFile << "#                                                                     #" << endl;
					resultsOutputFile << "#######################################################################" << endl << endl << endl;
				}
				else {
					resultsOutputFile << "#######################################################################" << endl;
					resultsOutputFile << "#  SAMPLING ANALYSIS RESULTS, LIMIT-STATE FUNCTION NUMBER   "
						<<setiosflags(ios::left)<<setprecision(1)<<setw(4)<<lsf <<"      #" << endl;
					resultsOutputFile << "#                                                                     #" << endl;
					resultsOutputFile << "#  Estimated mean: .................................... " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<q_bar(lsf-1) 
						<< "  #" << endl;
					resultsOutputFile << "#  Estimated standard deviation: ...................... " 
						<<setiosflags(ios::left)<<setprecision(5)<<setw(12)<<responseStdv(lsf-1) 
						<< "  #" << endl;
					resultsOutputFile << "#                                                                     #" << endl;
					resultsOutputFile << "#######################################################################" << endl << endl << endl;
				}
			}
		}

		if (analysisTypeTag == 2) {
			resultsOutputFile << "#######################################################################" << endl;
			resultsOutputFile << "#  RESPONSE CORRELATION COEFFICIENTS                                  #" << endl;
			resultsOutputFile << "#                                                                     #" << endl;
			if (numLsf <=1) {
				resultsOutputFile << "#  Only one limit-state function!                                     #" << endl;
			}
			else {
				resultsOutputFile << "#   gFun   gFun     Correlation                                       #" << endl;
				resultsOutputFile.setf(ios::fixed, ios::floatfield);
				for (i=0; i<numLsf; i++) {
					for (int j=i+1; j<numLsf; j++) {
						resultsOutputFile << "#    " <<setw(3)<<(i+1)<<"    "<<setw(3)<<(j+1)<<"     ";
						if (responseCorrelation(i,j)<0.0) { resultsOutputFile << "-"; }
						else { resultsOutputFile << " "; }
						resultsOutputFile <<setprecision(7)<<setw(11)<<fabs(responseCorrelation(i,j));
						resultsOutputFile << "                                      #" << endl;
					}
				}
			}
			resultsOutputFile << "#                                                                     #" << endl;
			resultsOutputFile << "#######################################################################" << endl << endl << endl;
		}

	}





	// Print summary of results to screen 
	cout << "Simulation Analysis completed." << endl;

	// Clean up
	resultsOutputFile.close();
	resultsOutputFile_aa.close();
    resultsOutputFile_bb.close();
	resultsOutputFile_cc.close();
    resultsOutputFile_dd.close();

 	out.close();


	delete theMatrixOperations;
	delete aStdNormRV;

	return 0;
}







int 
ImportanceSamplingAnalysis::correct(void)
{

	// Alert the user that the simulation analysis has started
	cout << "Importance Sampling correction is running ... " << endl;

   int k;
   int seed;
   double q=0;
   double pfIn, CovIn;
   double sum_q=0.0;
   double var_qbar =0.0;
   double sum_q_squared =0.0;
   char string[60];   

   	int numRV = theReliabilityDomain->getNumberOfRandomVariables();
	Vector x(numRV);


	ifstream inputFile( "_restartFile_.out", ios::in );
	inputFile >> k;
	inputFile >> seed;
	if (k==1 && seed==1) {
	}
	else { 
	 	inputFile >> pfIn;
		inputFile >> CovIn;
		sum_q = pfIn*k;
		var_qbar = (CovIn*pfIn)*(CovIn*pfIn);
		sum_q_squared = k*( k*var_qbar + pow(sum_q/k,2.0) );
	 
	k++;
	}
	inputFile.close();
	 

    ifstream resultsOutputFile_bb( "recorded_sampling_points_diverged_.out", ios::in );
    ofstream resultsOutputFile_bb2( "recorded_sampling_points_diverged_2.out", ios::out );

 
		char buffer [33];
		char tclAssignment[50];
		char theValueString[40];
		const char * myStr;
		double gFunctionValue =0.0;
		bool  FEconvergence = true; 
        double I =0;
        double q_bar=0.0;
		double variance_of_q_bar =0.0;
		double cov_of_q_bar =0.0;

	while( !resultsOutputFile_bb.eof()) {

		 resultsOutputFile_bb>>q ;

		cout << "correct Sample #" << k << ":" << endl;

		for ( int i =0; i<numRV; i++);
		   // resultsOutputFile_bb>>x(i);

// ---(1) set _RV_(1) 0.3; set _RV_(2) 0.4; ...
// ---(2) source _correction_.tcl
// ---(3)  get _gValue_


		// --- (1)	

		for (int ii =0; ii<numRV; ii++){
			(char*)_itoa (ii+1,buffer,10);
			strcpy (tclAssignment, "set _RV_(");				
			strcat (tclAssignment, buffer);
			strcat (tclAssignment, ")  ");
			sprintf(theValueString," %20.14e", x(ii));
			strcat (tclAssignment, theValueString);
		    if (Tcl_Eval(theInterp,tclAssignment) !=TCL_OK ){
  					cout<<"Fatal,IS--FEconvergence, can not set varuable with name:  _RV_("<<ii<<"in tcl command!"<<endl;  
				cout<<"command is:"<<tclAssignment<<endl;
				exit(-1);
			} //if

				
		}// for (int ii =0; ii<numRV; ii++)

         // --- (2)

		strcpy (tclAssignment, "source _correction_.tcl");
		if (Tcl_Eval(theInterp,tclAssignment) ==TCL_OK ){
   					
			// --- (3)


			myStr = Tcl_GetVar(theInterp, "_gValue_",TCL_GLOBAL_ONLY );

			if (myStr == "N") {  // not converge
				gFunctionValue =0.0;
				FEconvergence = false;
			}
			else {
				gFunctionValue = atof(myStr);
				FEconvergence = true; 
			}
		}
		else {
		    cout<<"Fatal,IS--FEconvergence, user did not provide _correction_.tcl !"<<endl;  
			//exit(-1);
		}


 
		
		// Collect result of sampling
		if (gFunctionValue < 0.0) {
			I = 1;
 
		}
		else {
			I = 0;
		}


		if (!FEconvergence) {  // recorder point and skip this point

			resultsOutputFile_bb2.precision(16);
			resultsOutputFile_bb2<<q<<"  \n  ";

			for (int i=0; i<x.Size();i++)	resultsOutputFile_bb2<<x(i)<<"  \n  ";
			resultsOutputFile_bb2.flush();

			cout<<"warning: bad point "<<k<<" is ignored! valid for ONLY one limit state function! "<<endl; 
			k--; 

			
		} //if(!FEconvergence)
					
	

		else {  // FEconvergence
			sum_q +=  q;
			sum_q_squared +=  q*q;


		 
			// Compute coefficient of variation (of pf)
			q_bar = 1.0/(double)k * sum_q;
			variance_of_q_bar = 1.0/(double)k * 
					( 1.0/(double)k * sum_q_squared - (sum_q/(double)k)*(sum_q/(double)k));
			if (variance_of_q_bar < 0.0) {
				variance_of_q_bar = 0.0;
			}
			cov_of_q_bar = sqrt(variance_of_q_bar) / q_bar;
				 
		} //else

 
		
		// Compute governing coefficient of variation
 

		// Print to the restart file, if requested. 
	
		ofstream outputFile( "_restartFile_.out", ios::out );
		outputFile << k << endl;
		outputFile << seed << endl;
	 
		sprintf(string,"%15.10f \n %15.10f",q_bar,cov_of_q_bar);
		outputFile << string << " " << endl;
	 
        outputFile.close();
        cout<<"Prob:"<<q_bar<<"        cov:"<<cov_of_q_bar<<endl;	

		// Increment k (the simulation number counter)
	//	if (FEconvergence ||(analysisTypeTag != 4) ){
	//	if (analysisTypeTag != 4 ){

		k++;

				
 

	} // while( (k<=numberOfSimulations && govCov>targetCOV || k<=2) )

	// Step 'k' back a step now that we went out
	k--;
	cout << endl;




	// Print summary of results to screen 
	cout << "This dirty Correction is completed. " << endl;

 
    resultsOutputFile_bb.close();
	resultsOutputFile_bb2.close();
 

 


 
	return 0;
}