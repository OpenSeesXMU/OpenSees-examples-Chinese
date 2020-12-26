//
// Written by Weiming Chen 
//
#include <iostream>
#include "SubsetSimulationAnalysis.h"
#include "ProbabilityTransformation.h"
#include "NatafProbabilityTransformation.h"
#include "RandomNumberGenerator.h"
#include "RandomVariable.h"
#include "RandomVariablePositioner.h"
#include "GFunEvaluator.h"
#include "NormalRV.h"
#include "Vector.h"
#include "Matrix.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include<ShellAPI.h>    // ShellExecuteEx
#include "MarkovChainGenerator.h"
#include <algorithm>
using namespace std;

using std::ifstream;

Vector BubbleSort(Vector p,int *index)
{
	int length;
	length = p.Size();
	for (int m = 0; m < length; m++) {
		*(index+m) = m;
	}

	for (int i = 0; i < length; i++) {

		for (int j = 0; j < length - i - 1; j++) {

			if ( p[j] > p[j + 1]) {

				double temp = p[j];
				p[j] = p[j + 1];
				p[j + 1] = temp;
				int ind_temp = *(index +j);
				*(index + j) = *(index + j+1);
				*(index + j + 1) = ind_temp;
			}
		}
	}
	return p;
}

double Prctile(Vector x, double p) {
	int length;
	length = x.Size();
	Vector Index(length);
	for (int i = 0; i < length; i++) {
		Index[i] = 100 * (i + 1 - 0.5) / length;
	}

	int *ptr_x;
	ptr_x = new int[length];
	x = BubbleSort(x, ptr_x);

	double Output;
	if (p <= Index[0]) {
		Output = x[0];
	}
	else if (p >= Index[length - 1]) {
		Output = x[length-1];
	}
	else {
		int Rindex;
		bool StopFlag = false;
		int j = 0;
		while (!StopFlag) {
			if (Index[j] > p) {
				Rindex = j;
				StopFlag = true;
			}
			j++;
		}
		Output = (x[Rindex] - x[Rindex - 1]) / (Index[Rindex] - Index[Rindex - 1])*(p - Index[Rindex - 1]) + x[Rindex - 1];
	}
	delete ptr_x;
	return Output;
}


SubsetSimulationAnalysis::SubsetSimulationAnalysis(
	GFunEvaluator *passedGFunEvaluator,
	ReliabilityDomain *passedReliabilityDomain,
	Tcl_Interp *passedTclInterp,
	ProbabilityTransformation *passedProbabilityTransformation,
	RandomNumberGenerator *passedRandomNumberGenerator,
	int passedPrintFlag,
	TCL_Char *passedFileName,
	int pSeed,
	TCL_Char *samplingType,
	int samplingFlag,
	int numSeedSamples
)
{
	theGFunEvaluator = passedGFunEvaluator;
	theReliabilityDomain = passedReliabilityDomain;
	theTclInterp = passedTclInterp;
	theProbabilityTransformation = passedProbabilityTransformation;
	theRandomNumberGenerator = passedRandomNumberGenerator;
	printFlag = passedPrintFlag;
	strcpy(fileName, passedFileName);
	seed = pSeed;
	strcpy(SamplingType, samplingType);
	SamplingFlag = samplingFlag;
	NumSeedSamples = numSeedSamples;
}


SubsetSimulationAnalysis::~SubsetSimulationAnalysis()
{
	//if (tclFileToRun !=0) delete [] tclFileToRun;
}


int SubsetSimulationAnalysis::analyze() {
	cout << "Subset Simulation Analysis is running ... " << endl;
	int result;
	bool isFirstGeneration = true;
	bool failureHasOccured = false;

	if (printFlag == 2) {

		ifstream inputFile("_restart.tmp", ios::in);
		if (!inputFile) {
			// file doesn't exist; so it's safe to create it and write to it
		}
		else {
			inputFile >> seed;
			inputFile.close();
			isFirstGeneration = false;
		}

	}

	// Prepare output file
	ofstream resultsOutputFile(fileName, ios::out); //Outputs the variables in the original space

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
	ofstream resultsOutputFile_1(fileName, ios::out); //Save the variables in standard normal space
	ofstream resultsOutputFile_2("AcceptanceRate.out", ios::out);//Save acceptance rate
	ofstream resultsOutputFile_3("CumulativeProbabilityDensity.out", ios::out);//Save cumulative probability density function

	resultsOutputFile.precision(15);
	resultsOutputFile_1.precision(15);
	resultsOutputFile_2.precision(15);
	resultsOutputFile_3.precision(3);

	double Rio = 0.1; //Parameter of SS, typically within [0.1,0.3]
	int NumIterations = 10; //Maximum iterations of subset simulation
	double pi = 3.14159265358979;
	double t_f = pi / 4;
	bool StopFlag = false;
	int Iteration = 0;
	int NumCallgFun = 0;
	double IntermediategFunValue;
	double P_f;
	double Cov;
	int numRV = theReliabilityDomain->getNumberOfRandomVariables();
	int NumSelectedSeedSamples;
	NumSelectedSeedSamples = (int)(Rio*NumSeedSamples);
	Matrix InitialSeedSamples(NumSeedSamples, numRV);//Standard normal space
	Matrix SelectedSeedSamples(NumSelectedSeedSamples, numRV);//Standard normal space
	Matrix InitialSeedSamples_x(NumSeedSamples, numRV);//Original space 
	Matrix SelectedSeedSamples_x(NumSelectedSeedSamples, numRV);//Original space
	Vector InitialgFunValue(NumSeedSamples);
	Vector SelectedgFunValue(NumSelectedSeedSamples);
	

	//Generate the initial seed samples by independent sampling method
	for (int i = 0; i < NumSeedSamples; i++) {
		//Create array of standard normal random numbers
		if (isFirstGeneration) {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV, seed);
			isFirstGeneration = false;
		}
		else {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(numRV);
		}
		seed = theRandomNumberGenerator->getSeed();
		if (result < 0) {
			cout << "SubsetSimulation::analyze() - could not generate" << endl
				<< " random numbers for initial seed sample." << endl;
			return -1;
		}
		Vector u(numRV);
		u = theRandomNumberGenerator->getGeneratedNumbers();

		// Transform into original space
		result = theProbabilityTransformation->set_u(u);
		if (result < 0) {
			cout << "SubsetSimulationAnalysis::analyze() - could not " << endl
				<< " set the u-vector for xu-transformation. " << endl;
			return -1;
		}

		result = theProbabilityTransformation->transform_u_to_x();
		if (result < 0) {
			cout << "SubsetSimulationAnalysis::analyze() - could not " << endl
				<< " transform u to x. " << endl;
			return -1;
		}
		Vector x(numRV);
		x = theProbabilityTransformation->get_x();

		//compute structural response and gfunction
		result = theGFunEvaluator->runGFunAnalysis(x);
		result = theGFunEvaluator->evaluateG(x);
		InitialgFunValue[i] = theGFunEvaluator->getG();
		NumCallgFun++;

		for (int j = 0; j < numRV; j++) {
			resultsOutputFile << x(j) << endl;
			resultsOutputFile_1 << u(j) << endl;
			InitialSeedSamples_x(i, j) = x(j);
			InitialSeedSamples(i, j) = u(j);
		}
		resultsOutputFile << "k:" << i << ",  " << "gFunValue:" << InitialgFunValue[i] << endl;
		resultsOutputFile_1 << "k:" << i << ",  " << "gFunValue:" << InitialgFunValue[i] << endl;
		cout << "Subset #"<< 0 <<",  "<<"Sample #" << i << ",  " << "gFunValue:" << InitialgFunValue[i] << endl;
	}

	while (!StopFlag) {

		//selects seed samples from the initial seed samples
		int *Index;
		Index = new int[NumSeedSamples];
		InitialgFunValue = BubbleSort(InitialgFunValue, Index);
		for (int i = 0; i < NumSelectedSeedSamples; i++) {

			for (int j = 0; j < numRV; j++) {
				SelectedSeedSamples(i, j) = InitialSeedSamples(*(Index+i), j);
				SelectedSeedSamples_x(i, j) = InitialSeedSamples_x(*(Index + i), j);
			}

			SelectedgFunValue(i) = InitialgFunValue(i);
		}
		delete Index;

		//Determines subset
		IntermediategFunValue = Prctile(InitialgFunValue,Rio*100);

		if (IntermediategFunValue < 0.0 || Iteration > NumIterations) {
			int I = 0;
			for (int i = 0; i < NumSeedSamples; i++) {
				if (InitialgFunValue[i] < 0.0) {
					I++;
				}
			}
			P_f = pow(Rio, Iteration)*I / NumSeedSamples;

			if (Iteration == 0) {
				resultsOutputFile_3 << IntermediategFunValue << " " << P_f << endl;
			}
			else {
				resultsOutputFile_3 << 0 << " " << P_f << endl;
			}
			
			StopFlag = true;
		}
		else {
			Iteration++;
			resultsOutputFile_3 << IntermediategFunValue << " " << pow(Rio, Iteration) << endl;

			//Randomly sort SelectedgFunValue
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(NumSelectedSeedSamples);
			seed = theRandomNumberGenerator->getSeed();
			Vector RandomVector(NumSelectedSeedSamples);
			RandomVector = theRandomNumberGenerator->getGeneratedNumbers();
			int *RandomIndex;
			RandomIndex = new int[NumSelectedSeedSamples];
			RandomVector = BubbleSort(RandomVector, RandomIndex);


			int NumAcception = 0;
			//Generates new InitialSeedSamples by Markov Chain Monte Carlo samplimg method
			for (int i = 0; i < NumSelectedSeedSamples; i++) {
				int CurrentIndex;
				CurrentIndex = *(RandomIndex + i);
				Vector CurrentSeedSample(numRV);
				Vector CurrentSeedSample_x(numRV);
				double CurrentgFunValue;
				for (int j = 0; j < numRV; j++) {
					CurrentSeedSample[j] = SelectedSeedSamples(CurrentIndex, j);
					CurrentSeedSample_x[j] = SelectedSeedSamples_x(CurrentIndex, j);
				}
				CurrentgFunValue = SelectedgFunValue(CurrentIndex);


				for (int j = 0; j < numRV; j++) {
					InitialSeedSamples((int)(i / Rio), j) = CurrentSeedSample[j];
					InitialSeedSamples_x((int)(i / Rio), j) = CurrentSeedSample_x[j];
					resultsOutputFile << CurrentSeedSample_x[j] << endl;
					resultsOutputFile_1 << CurrentSeedSample[j] << endl;
				}
				InitialgFunValue[(int)(i / Rio)] = CurrentgFunValue;

				resultsOutputFile << "k:" << (int)(i / Rio) << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio)] << endl;
				resultsOutputFile_1 << "k:" << (int)(i / Rio) << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio)] << endl;
				cout << "Subset #" << Iteration << ",  " << "Sample #" << (int)(i / Rio) << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio)] << endl;


				for (int j = 1; j < 1 / Rio; j++) {

					Vector NewSeedSample(numRV);
					MarkovChainGenerator *theMarkovChainGenerator = 0;
					theMarkovChainGenerator = new MarkovChainGenerator(
						theRandomNumberGenerator,
						isFirstGeneration,
						seed,
						SamplingType,
						SamplingFlag,
						t_f
					);
					theMarkovChainGenerator->GenerateNewSample(CurrentSeedSample);
					NewSeedSample = theMarkovChainGenerator->NewSample;
					seed = theMarkovChainGenerator->seed;
					delete theMarkovChainGenerator;

					if (NewSeedSample == CurrentSeedSample) {
						for (int k = 0; k < numRV; k++) {
							InitialSeedSamples((int)(i / Rio) + j, k) = CurrentSeedSample[k];
							InitialSeedSamples_x((int)(i / Rio) + j, k) = CurrentSeedSample_x[k];
							resultsOutputFile << CurrentSeedSample_x[k] << endl;
							resultsOutputFile_1 << CurrentSeedSample[k] << endl;
						}
						InitialgFunValue[(int)(i / Rio) + j] = CurrentgFunValue;
						resultsOutputFile << "k:" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
						resultsOutputFile_1 << "k:" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
						cout << "Subset #" << Iteration << ",  " << "Sample #" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
					}
					else {
						
						// Transform into original space
						result = theProbabilityTransformation->set_u(NewSeedSample);
						if (result < 0) {
							cout << "SubsetSimulationAnalysis::analyze() - could not " << endl
								<< " set the u-vector for xu-transformation. " << endl;
							return -1;
						}

						result = theProbabilityTransformation->transform_u_to_x();
						if (result < 0) {
							cout << "SubsetSimulationAnalysis::analyze() - could not " << endl
								<< " transform u to x. " << endl;
							return -1;
						}
						Vector x(numRV);
						x = theProbabilityTransformation->get_x();

						//compute structural response and gfunction
						result = theGFunEvaluator->runGFunAnalysis(x);
						result = theGFunEvaluator->evaluateG(x);
						double NewgFunValue;
						NewgFunValue = theGFunEvaluator->getG();
						NumCallgFun++;

						
						//Accept or reject
						if (NewgFunValue <= IntermediategFunValue) {
							NumAcception++;
							for (int k = 0; k < numRV; k++) {
								resultsOutputFile << x(k) << endl;
								resultsOutputFile_1 << NewSeedSample(k) << endl;
								InitialSeedSamples((int)(i / Rio) + j, k) = NewSeedSample(k);
								InitialSeedSamples_x((int)(i / Rio) + j, k) = x(k);
							}
							InitialgFunValue[(int)(i / Rio) + j] = NewgFunValue;
							CurrentSeedSample = NewSeedSample;
							CurrentSeedSample_x = x;
							CurrentgFunValue = NewgFunValue;
						}
						else {
							for (int k = 0; k < numRV; k++) {
								InitialSeedSamples((int)(i / Rio) + j, k) = CurrentSeedSample[k];
								InitialSeedSamples_x((int)(i / Rio) + j, k) = CurrentSeedSample_x[k];
								resultsOutputFile << CurrentSeedSample_x[k] << endl;
								resultsOutputFile_1 << CurrentSeedSample[k] << endl;
							}
							InitialgFunValue[(int)(i / Rio) + j] = CurrentgFunValue;
						}
						resultsOutputFile << "k:" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
						resultsOutputFile_1 << "k:" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
						cout << "Subset #" << Iteration << ",  " << "Sample #" << (int)(i / Rio) + j << ",  " << "gFunValue:" << InitialgFunValue[(int)(i / Rio) + j] << endl;
					}
				}
				

				//Modify t_f
				if ((i + 1) % (int)(0.1*NumSelectedSeedSamples) == 0) {

					double AcceptanceRate;
					AcceptanceRate = NumAcception / (NumSelectedSeedSamples/10 * (1 / Rio - 1));
					if (AcceptanceRate < 0.3) {
						t_f = asin(sin(t_f)*exp((AcceptanceRate - 0.3) / 2));
					}
					else if (AcceptanceRate > 0.5) {
						t_f = asin(sin(t_f)*exp((AcceptanceRate - 0.5) / 2));
					}
					NumAcception = 0;
					resultsOutputFile_2 << AcceptanceRate <<", "<< t_f << endl;
				}
			}
			delete RandomIndex;
		}
	}

	// Print summary of results to screen 
	cout << "Subset Simulation Analysis completed." << endl;

	//Evaluate the failure probability
	cout << "Pf = " << P_f << endl;
	cout << "NG = " << NumCallgFun << endl;
	resultsOutputFile.close();
	resultsOutputFile_1.close();
	resultsOutputFile_2.close();
	resultsOutputFile_3.close();
	return 0;
};