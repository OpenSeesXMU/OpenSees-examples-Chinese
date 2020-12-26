#pragma once
// SubsetSimulationAnalysis.h: interface for the SubsetSimulationAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SUBSETSIMULATIONANALYSIS
#define SUBSETSIMULATIONANALYSIS

#include "reliabilityDomain.h"
#include "ProbabilityTransformation.h"
#include "RandomNumberGenerator.h"
#include "GFunEvaluator.h"
#include <tcl.h>

class SubsetSimulationAnalysis
{
public:
	SubsetSimulationAnalysis(GFunEvaluator *passedGFunEvaluator,
		ReliabilityDomain *passedReliabilityDomain,
		Tcl_Interp *passedTclInterp,
		ProbabilityTransformation *passedProbabilityTransformation,
		RandomNumberGenerator *passedRandomNumberGenerator,
		int printFlag,
		TCL_Char *outputFileName,
		int seed,
		TCL_Char *SamplingType,
		int SamplingFlag,
		int NumSeedSamples //The number of seed samples in each intermediate state of SS
	);
	virtual ~SubsetSimulationAnalysis();
	int analyze();

private:
	GFunEvaluator *theGFunEvaluator;
	ReliabilityDomain *theReliabilityDomain;
	Tcl_Interp *theTclInterp;
	ProbabilityTransformation *theProbabilityTransformation;
	RandomNumberGenerator *theRandomNumberGenerator;
	int printFlag;
	char fileName[25];
	int seed;
	char SamplingType[25];
	int SamplingFlag;
	int NumSeedSamples;
};

#endif 
