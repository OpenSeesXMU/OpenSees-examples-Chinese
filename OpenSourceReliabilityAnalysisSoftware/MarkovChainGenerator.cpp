//
// Written by Weiming Chen 
//

#include <iostream>
#include "RandomNumberGenerator.h"
#include "MarkovChainGenerator.h"
#include "NormalRV.h"
#include "Vector.h"
#include "Matrix.h"
#include <math.h>
#include <tcl.h>
#include <algorithm>

using namespace std;
using std::ifstream;

MarkovChainGenerator::MarkovChainGenerator(RandomNumberGenerator *passedRandomNumberGenerator,
	bool pisFirstGeneration,
	int pseed,
	TCL_Char *pSamplingType,
	int pSamplingFlag,
	double t) {
	theRandomNumberGenerator = passedRandomNumberGenerator;
	isFirstGeneration = pisFirstGeneration;
	seed = pseed;
	SamplingType = pSamplingType;
	SamplingFlag = pSamplingFlag;
	t_f = t;
}

MarkovChainGenerator::~MarkovChainGenerator() {
	//Does nothing
}

int MarkovChainGenerator::GenerateNewSample(Vector x) {
	int NumRV;
	NumRV = x.Size();
	Vector q_old(NumRV);
	Vector p_old(NumRV);
	Vector q_new(NumRV);
	int result;

	q_old = x;

	if (strcmp(SamplingType, "HMC") == 0) {
		// Create array of standard normal random numbers
		if (isFirstGeneration) {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(NumRV, seed);
		}
		else {
			result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(NumRV);
		}
		seed = theRandomNumberGenerator->getSeed();
		if (result < 0) {
			cout << "MarkovChainGenerator::GenerateNewSample() - could not generate" << endl
				<< " random numbers for simulation." << endl;
			return -1;
		}
		p_old = theRandomNumberGenerator->getGeneratedNumbers();
		q_new = p_old * sin(t_f) + q_old * cos(t_f);
		NewSample = q_new;
	}
	else {
		// Create array of standard normal random numbers
		if (SamplingFlag == 1) {
			if (isFirstGeneration) {
				result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(NumRV, seed);
			}
			else {
				result = theRandomNumberGenerator->generate_nIndependentStdNormalNumbers(NumRV);
			}
		}
		else {
			if (isFirstGeneration) {
				result = theRandomNumberGenerator->generate_nIndependentUniformNumbers(NumRV, -1, 1, seed);
			}
			else {
				result = theRandomNumberGenerator->generate_nIndependentUniformNumbers(NumRV, -1, 1);
			}
		}
		seed = theRandomNumberGenerator->getSeed();
		if (result < 0) {
			cout << "MarkovChainGenerator::GenerateNewSample() - could not generate" << endl
				<< " random numbers for simulation." << endl;
			return -1;
		}
		Vector randomArray(NumRV);
		Vector q_new1(NumRV);
		randomArray = theRandomNumberGenerator->getGeneratedNumbers();
		q_new1 = q_old + randomArray;


		NormalRV *aStdNormRV = 0;
		aStdNormRV = new NormalRV(1, 0.0, 1.0, 0.0);

		for (int i = 0; i < NumRV; i++) {
			double q_new_pdf;
			double q_old_pdf;
			q_new_pdf = aStdNormRV->getPDFvalue(q_new1[i]);
			q_old_pdf = aStdNormRV->getPDFvalue(q_old[i]);
			double alpha;
			alpha= min(1.0, q_new_pdf/ q_old_pdf);
			result = theRandomNumberGenerator->generate_nIndependentUniformNumbers(1, 0, 1);
			if (result < 0) {
				cout << "MarkovChainGenerator::GenerateNewSample() - could not generate" << endl
					<< " random numbers for u." << endl;
				return -1;
			}
			seed = theRandomNumberGenerator->getSeed();
			Vector u(1);
			u = theRandomNumberGenerator->getGeneratedNumbers();
			if (u[0] <= alpha) {
				q_new[i] = q_new1[i];
			}
			else {
				q_new[i] = q_old[i];
			}
		}
		NewSample = q_new;
		delete aStdNormRV;
	}
	return 0;
};

