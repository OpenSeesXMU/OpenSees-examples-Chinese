//
// Written by Weiming Chen 
//

#pragma once

#ifndef MARKOVCHAINGENERATOR
#define MARKOVCHAINGENERATOR

#include "RandomNumberGenerator.h"
#include "NormalRV.h"
#include "Vector.h"
#include "Matrix.h"
#include <math.h>
#include <tcl.h>

using namespace std;

class MarkovChainGenerator {

public:
	MarkovChainGenerator(RandomNumberGenerator *passedRandomNumberGenerator,
		bool pisFirstGenetation,
		int pseed,
		TCL_Char *pSamplingType,
		int pSamplingFlag,
		double t);
	~MarkovChainGenerator();
	int seed;
	int GenerateNewSample(Vector x);
	Vector NewSample;
private:
	RandomNumberGenerator *theRandomNumberGenerator;
	bool isFirstGeneration;
	TCL_Char *SamplingType;
	int SamplingFlag;
	double t_f;
};

#endif