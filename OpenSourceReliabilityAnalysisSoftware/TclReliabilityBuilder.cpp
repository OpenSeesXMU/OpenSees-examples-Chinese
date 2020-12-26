#include "tcl.h"
#include "Vector.h"
#include "ReliabilityDomain.h"
#include "TclReliabilityBuilder.h"
#include "NormalRV.h"
#include "LognormalRV.h"
#include "GammaRV.h"
#include "ShiftedExponentialRV.h"
#include "ShiftedRayleighRV.h"
#include "ExponentialRV.h"
#include "UniformRV.h"
#include "BetaRV.h"
#include "Type1LargestValueRV.h"
#include "Type1SmallestValueRV.h"
#include "Type2LargestValueRV.h"
#include "Type3SmallestValueRV.h"
#include "TruncatedNormalRV.h"
#include "RayleighRV.h"
#include "ChiSquareRV.h"
#include "GumbelRV.h"
#include "WeibullRV.h"
#include "LaplaceRV.h"
#include "ParetoRV.h"
#include "CorrelationCoefficient.h"
#include "LimitStateFunction.h"
#include "GFunEvaluator.h"
#include "ArrayOfTaggedObjects.h"
#include "ProbabilityTransformation.h"
#include "NatafProbabilityTransformation.h"
#include "ReliabilityConvergenceCheck.h"
#include "StandardReliabilityConvergenceCheck.h"
#include "OptimalityConditionReliabilityConvergenceCheck.h"
#include "BasicGFunEvaluator.h"
#include "OpenSeesGFunEvaluator.h"
#include "GradGEvaluator.h"
#include "FiniteDifferenceGradGEvaluator.h"
#include "SearchDirection.h"
#include "HLRFSearchDirection.h"
#include "MeritFunctionCheck.h"
#include "AdkZhangMeritFunctionCheck.h"
#include "StepSizeRule.h"
#include "ArmijoStepSizeRule.h"
#include "FindDesignPointAlgorithm.h"
#include "HessianApproximation.h"
#include "SearchWithStepSizeAndStepDirection.h"
#include "ReliabilityAnalysis.h"
#include "FORMAnalysis.h"
#include "FOSMAnalysis.h"
#include "FindCurvatures.h"
#include "FirstPrincipalCurvature.h"
#include "CurvaturesBySearchAlgorithm.h"
#include "SORMAnalysis.h"
#include "RootFinding.h"
#include "SecantRootFinding.h"
#include "FixedStepSizeRule.h"
#include "GradientProjectionSearchDirection.h"
#include "PolakHeSearchDirectionAndMeritFunction.h"
#include "SQPsearchDirectionMeritFunctionAndHessian.h"
#include "RandomNumberGenerator.h"
#include "CStdLibRandGenerator.h"
#include "MersenneTwister.h"
#include "MonteCarloResponseAnalysis.h"
#include "ImportanceSamplingAnalysis.h"
#include "GFunVisualizationAnalysis.h"
#include "SystemAnalysis.h"
#include "Filter.h"
#include "DeltaFilter.h"
#include "StandardLinearOscillatorDisplacementFilter.h"
#include "KooFilter.h"
#include "StandardLinearOscillatorVelocityFilter.h"
#include "StandardLinearOscillatorAccelerationFilter.h"
#include "ModulatingFunction.h"
#include "GammaModulatingFunction.h"
#include "ConstantModulatingFunction.h"
#include "TrapezoidalModulatingFunction.h"
#include "KooModulatingFunction.h"
#include "Spectrum.h"
#include "JonswapSpectrum.h"
#include "NarrowBandSpectrum.h"
#include "PointsSpectrum.h"
#include "ParametricReliabilityAnalysis.h"
#include "OutCrossingAnalysis.h"
#include "SamplingAnalysis.h"
#include "OrthogonalPlaneSamplingAnalysis.h"
#include "SubsetSimulationAnalysis.h"
#include <iostream>
#include<stdlib.h>
#include<Windows.h>
using namespace std;
Vector param_temp(4);
///////////////////////////////////////////////////////////
int reliability(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int TclReliabilityModelBuilder_addRandomVariable(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int TclReliabilityModelBuilder_addCorrelate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_correlateGroup(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_correlationStructure(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addLimitState(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addProbabilityTransformation(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addReliabilityConvergenceCheck(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addgFunEvaluator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addGradGEvaluator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addSearchDirection(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addMeritFunctionCheck(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addStepSizeRule(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addStartPoint(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addFindDesignPointAlgorithm(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runFORMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runFOSMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runSORMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addRootFinding(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addRandomNumberGenerator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runImportanceSamplingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runGFunVisualizationAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_runSystemAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
int TclReliabilityModelBuilder_addFindCurvatures(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runStructureAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runMonteCarloResponseAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runParametricReliabilityAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_addModulatingFunction(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_addFilter(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_addSpectrum(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runOutCrossingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runOrthogonalPlaneSamplingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);
int TclReliabilityModelBuilder_runSubsetSimulationAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv);




ReliabilityDomain *theReliabilityDomain = 0;
static Domain *theStructuralDomain = 0;
SamplingAnalysis * theSamplingAnalysis = 0;
static GFunEvaluator *theGFunEvaluator = 0;
static GradGEvaluator *theGradGEvaluator = 0;
static StepSizeRule *theStepSizeRule = 0;
static SearchDirection *theSearchDirection = 0;
static HessianApproximation *theHessianApproximation = 0;
static MeritFunctionCheck *theMeritFunctionCheck = 0;
static PolakHeSearchDirectionAndMeritFunction *thePolakHeDualPurpose = 0;
static SQPsearchDirectionMeritFunctionAndHessian *theSQPtriplePurpose = 0;
static ProbabilityTransformation *theProbabilityTransformation = 0;
static ReliabilityConvergenceCheck *theReliabilityConvergenceCheck = 0;
static Vector *theStartPoint = 0;
static RootFinding *theRootFindingAlgorithm = 0;
RandomNumberGenerator *theRandomNumberGenerator = 0;
static FindDesignPointAlgorithm *theFindDesignPointAlgorithm = 0;
static FORMAnalysis *theFORMAnalysis = 0;
static FOSMAnalysis *theFOSMAnalysis = 0;
static FindCurvatures *theFindCurvatures = 0;
static SORMAnalysis *theSORMAnalysis = 0;
static ImportanceSamplingAnalysis *theImportanceSamplingAnalysis = 0;
static GFunVisualizationAnalysis *theGFunVisualizationAnalysis = 0;
static ParametricReliabilityAnalysis *theParametricReliabilityAnalysis = 0;
static OutCrossingAnalysis *theOutCrossingAnalysis = 0;
static SystemAnalysis *theSystemAnalysis = 0;
MonteCarloResponseAnalysis * theMonteCarloResponseAnalysis = 0;
static TclReliabilityBuilder *theReliabilityBuilder = 0;
SubsetSimulationAnalysis * theSubsetSimulationAnalysis = 0;

int Tcl_AppInit(Tcl_Interp *interp)
{
	fprintf(stderr, "\n\t\t\t\t Open Source Reliability Analysis Software (ORS) \n\n\n");
	Tcl_CreateCommand(interp, "reliability", (Tcl_CmdProc *)reliability, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	Tcl_CreateCommand(interp, "randomVariable", (Tcl_CmdProc *)TclReliabilityModelBuilder_addRandomVariable, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
	Tcl_CreateCommand(interp, "correlate", (Tcl_CmdProc *)TclReliabilityModelBuilder_addCorrelate, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "correlateGroup", (Tcl_CmdProc *)TclReliabilityModelBuilder_correlateGroup, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "correlationStructure", (Tcl_CmdProc *)TclReliabilityModelBuilder_correlationStructure, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "performanceFunction", (Tcl_CmdProc *)TclReliabilityModelBuilder_addLimitState, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "probabilityTransformation", (Tcl_CmdProc *)TclReliabilityModelBuilder_addProbabilityTransformation, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "reliabilityConvergenceCheck", (Tcl_CmdProc *)TclReliabilityModelBuilder_addReliabilityConvergenceCheck, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "gFunEvaluator", (Tcl_CmdProc *)TclReliabilityModelBuilder_addgFunEvaluator, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "gradGEvaluator", (Tcl_CmdProc *)TclReliabilityModelBuilder_addGradGEvaluator, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "searchDirection", (Tcl_CmdProc *)TclReliabilityModelBuilder_addSearchDirection, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "meritFunctionCheck", (Tcl_CmdProc *)TclReliabilityModelBuilder_addMeritFunctionCheck, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "stepSizeRule", (Tcl_CmdProc *)TclReliabilityModelBuilder_addStepSizeRule, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "startPoint", (Tcl_CmdProc *)TclReliabilityModelBuilder_addStartPoint, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "findDesignPoint", (Tcl_CmdProc *)TclReliabilityModelBuilder_addFindDesignPointAlgorithm, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runFORMAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runFORMAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runFOSMAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runFOSMAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runSORMAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runSORMAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "rootFinding", (Tcl_CmdProc *)TclReliabilityModelBuilder_addRootFinding, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "randomNumberGenerator", (Tcl_CmdProc *)TclReliabilityModelBuilder_addRandomNumberGenerator, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runImportanceSamplingAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runImportanceSamplingAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runGFunVizAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runGFunVisualizationAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runSystemAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runSystemAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "findCurvatures", (Tcl_CmdProc *)TclReliabilityModelBuilder_addFindCurvatures, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runStructureAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runStructureAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runMonteCarloResponseAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runMonteCarloResponseAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "modulatingFunction", (Tcl_CmdProc *)TclReliabilityModelBuilder_addModulatingFunction, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "filter", (Tcl_CmdProc *)TclReliabilityModelBuilder_addFilter, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "spectrum", (Tcl_CmdProc *)TclReliabilityModelBuilder_addSpectrum, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runParametricReliabilityAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runParametricReliabilityAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runOutCrossingAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runOutCrossingAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runOrthogonalPlaneSamplingAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runOrthogonalPlaneSamplingAnalysis, (ClientData)NULL, NULL);
	Tcl_CreateCommand(interp, "runSubsetSimulationAnalysis", (Tcl_CmdProc *)TclReliabilityModelBuilder_runSubsetSimulationAnalysis, (ClientData)NULL, NULL);
	//--Quan

 // set the static pointers in this file
	//theStructuralDomain = &passedDomain;
	theReliabilityDomain = new ReliabilityDomain();

	return TCL_OK;
}


///////////////////////////////////////////////
TclReliabilityBuilder::~TclReliabilityBuilder()
{
	// Delete objects
	if (theReliabilityDomain != 0)
		delete theReliabilityDomain;
	if (theGFunEvaluator != 0) {
		delete theGFunEvaluator;
		theGFunEvaluator = 0;
	}
	if (theGradGEvaluator != 0)
		delete theGradGEvaluator;
	if (theStepSizeRule != 0)
		delete theStepSizeRule;
	if (theSearchDirection != 0)
		delete theSearchDirection;
	if (theHessianApproximation != 0)
		delete theHessianApproximation;
	if (thePolakHeDualPurpose != 0)
		delete thePolakHeDualPurpose;
	if (theSQPtriplePurpose != 0)
		delete theSQPtriplePurpose;
	if (theMeritFunctionCheck != 0)
		delete theMeritFunctionCheck;
	if (theReliabilityConvergenceCheck != 0)
		delete theReliabilityConvergenceCheck;
	if (theProbabilityTransformation != 0)
		delete theProbabilityTransformation;
	if (theStartPoint != 0)
		delete theStartPoint;
	if (theRootFindingAlgorithm != 0)
		delete theRootFindingAlgorithm;
	if (theRandomNumberGenerator != 0)
		delete theRandomNumberGenerator;
	if (theFindDesignPointAlgorithm != 0)
		delete theFindDesignPointAlgorithm;
	if (theFindCurvatures != 0)
		delete theFindCurvatures;
	if (theFORMAnalysis != 0)
		delete theFORMAnalysis;
	if (theFOSMAnalysis != 0)
		delete theFOSMAnalysis;
	if (theSORMAnalysis != 0)
		delete theSORMAnalysis;
	if (theImportanceSamplingAnalysis != 0)
		delete theImportanceSamplingAnalysis;
	/*if (theParametricReliabilityAnalysis != 0)
		delete theParametricReliabilityAnalysis;*/
	if (theSystemAnalysis != 0)
		delete theSystemAnalysis;


	// Quan ---
	/*if (theSNOPTAnalysis != 0)
		delete theSNOPTAnalysis;
	if (theMonteCarloResponseAnalysis != 0)
		delete theMonteCarloResponseAnalysis;
	if (theSNOPT != 0)
		delete theSNOPT;
		*/

	// ---Quan 

	theReliabilityDomain = 0;
	theReliabilityDomain = 0;
	theGFunEvaluator = 0;
	theGradGEvaluator = 0;
	theStepSizeRule = 0;
	theSearchDirection = 0;
	theHessianApproximation = 0;
	thePolakHeDualPurpose = 0;
	theSQPtriplePurpose = 0;
	theMeritFunctionCheck = 0;
	theReliabilityConvergenceCheck = 0;
	theProbabilityTransformation = 0;
	theStartPoint = 0;
	theRootFindingAlgorithm = 0;
	theRandomNumberGenerator = 0;
	theFindDesignPointAlgorithm = 0;
	theFindCurvatures = 0;
	theFORMAnalysis = 0;
	theFOSMAnalysis = 0;
	theSORMAnalysis = 0;
	/*theParametricReliabilityAnalysis = 0;
	theImportanceSamplingAnalysis = 0;
	theSystemAnalysis = 0;
	*/



	// Delete commands
	Tcl_DeleteCommand(theInterp, "randomVariable");
	Tcl_DeleteCommand(theInterp, "correlate");
	Tcl_DeleteCommand(theInterp, "correlateGroup");
	Tcl_DeleteCommand(theInterp, "correlationStructure");
	Tcl_DeleteCommand(theInterp, "limitState");
	Tcl_DeleteCommand(theInterp, "randomVariablePositioner");
	Tcl_DeleteCommand(theInterp, "positionerPositioner");
	Tcl_DeleteCommand(theInterp, "modulatingFunction");
	Tcl_DeleteCommand(theInterp, "filter");
	Tcl_DeleteCommand(theInterp, "spectrum");
	Tcl_DeleteCommand(theInterp, "findDesignPoint");
	Tcl_DeleteCommand(theInterp, "gFunEvaluator");
	Tcl_DeleteCommand(theInterp, "GradGEvaluator");
	Tcl_DeleteCommand(theInterp, "stepSizeRule");
	Tcl_DeleteCommand(theInterp, "searchDirection");
	Tcl_DeleteCommand(theInterp, "HessianApproximation");
	Tcl_DeleteCommand(theInterp, "meritFunctionCheck");
	Tcl_DeleteCommand(theInterp, "reliabilityConvergenceCheck");
	Tcl_DeleteCommand(theInterp, "ProbabilityTransformation");
	Tcl_DeleteCommand(theInterp, "startPoint");
	Tcl_DeleteCommand(theInterp, "rootFinding");
	Tcl_DeleteCommand(theInterp, "findCurvatures");
	Tcl_DeleteCommand(theInterp, "randomNumberGenerator");
	Tcl_DeleteCommand(theInterp, "runFORMAnalysis");
	Tcl_DeleteCommand(theInterp, "runFOSMAnalysis");
	Tcl_DeleteCommand(theInterp, "runParametricReliabilityAnalysis");
	Tcl_DeleteCommand(theInterp, "runGFunVizAnalysis");
	Tcl_DeleteCommand(theInterp, "runSORMAnalysis");
	Tcl_DeleteCommand(theInterp, "runSystemAnalysis");
	Tcl_DeleteCommand(theInterp, "runImportanceSamplingAnalysis");
	Tcl_DeleteCommand(theInterp, "tempCommand");
	Tcl_DeleteCommand(theInterp, "inputCheck");
	Tcl_DeleteCommand(theInterp, "getMean");
	Tcl_DeleteCommand(theInterp, "getStdv");
	// ---- Quan
	Tcl_DeleteCommand(theInterp, "designVariable");
	Tcl_DeleteCommand(theInterp, "designVariablePositioner");
	Tcl_DeleteCommand(theInterp, "constraintFunction");
	Tcl_DeleteCommand(theInterp, "objectiveFunction");
	Tcl_DeleteCommand(theInterp, "runSNOPTAnalysis");
	Tcl_DeleteCommand(theInterp, "runMonteCarloResponseAnalysis");
	Tcl_DeleteCommand(theInterp, "updateParameter");
	Tcl_DeleteCommand(theInterp, "runOrthogonalPlaneSamplingAnalysis");
	Tcl_DeleteCommand(theInterp, "computeHessian");
	Tcl_DeleteCommand(theInterp, "runMultiDimVisualPrinPlane");
	Tcl_DeleteCommand(theInterp, "runDP_RSM_SimTimeInvariantAnalysis");
	Tcl_DeleteCommand(theInterp, "runDP_RSM_SimTimeVariantAnalysis");


}


//
// CLASS METHODS
//
ReliabilityDomain *
TclReliabilityBuilder::getReliabilityDomain()
{
	return theReliabilityDomain;
}


/////////////////////////////////////////////////////
int TclReliabilityModelBuilder_addRandomVariable(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]) {
	RandomVariable *theRandomVariable = 0;
	int tag;
	double mean;
	double stdv;
	double startPt;
	double parameter1;
	double parameter2;
	double parameter3;
	double parameter4;
	int numberOfArguments = argc;

	// GET TAG NUMBER
	if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
		cout << "ERROR: invalid input: tag \n";
		return TCL_ERROR;
	}

	// CHECK THAT AT LEAST ENOUGH ARGUMENTS ARE GIVEN
	if (numberOfArguments < 5) {
		cout << "ERROR: invalid number of arguments to randomVariable command \n";
		return TCL_ERROR;
	}


	// CHECK THAT THE USER HAS PROVIDED A TYPE
	if ((strcmp(argv[2], "beta") == 0) ||
		(strcmp(argv[2], "chiSquare") == 0) ||
		(strcmp(argv[2], "exponential") == 0) ||
		(strcmp(argv[2], "gamma") == 0) ||
		(strcmp(argv[2], "gumbel") == 0) ||
		(strcmp(argv[2], "laplace") == 0) ||
		(strcmp(argv[2], "lognormal") == 0) ||
		(strcmp(argv[2], "normal") == 0) ||
		(strcmp(argv[2], "pareto") == 0) ||
		(strcmp(argv[2], "rayleigh") == 0) ||
		(strcmp(argv[2], "shiftedExponential") == 0) ||
		(strcmp(argv[2], "shiftedRayleigh") == 0) ||
		(strcmp(argv[2], "type1LargestValue") == 0) ||
		(strcmp(argv[2], "type1SmallestValue") == 0) ||
		(strcmp(argv[2], "type2LargestValue") == 0) ||
		(strcmp(argv[2], "type3SmallestValue") == 0) ||
		(strcmp(argv[2], "uniform") == 0) ||
		(strcmp(argv[2], "userdefined") == 0) ||
		(strcmp(argv[2], "weibull") == 0) ||
		(strcmp(argv[2], "truncatedNormal") == 0)
		)
	{
	}
	else {
		cout << "ERROR: A correct type has not been provided for a random variable." << endl
			<< " (Available types: normal, lognormal, uniform, etc.)" << endl
			<< " Syntax: randomVariable tag? type mean? stdv? <startPt?>" << endl
			<< "     or: randomVariable tag? type par1? par2? par3? par4? <startPt?>" << endl;
		return TCL_ERROR;
	}


	
	
	

	// NOW START CREATING THE RANDOM VARIBLE OBJECT
	 if (numberOfArguments == 5) {   // (Use mean/stdv WITHOUT startPt)

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[3], &mean) != TCL_OK) {
			cout << "ERROR: invalid input: mean \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &stdv) != TCL_OK) {
			cout << "ERROR: invalid input: stdv \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
		if (strcmp(argv[2], "normal") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new NormalRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "lognormal") == 0) {
			if (mean == 0.0 || stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new LognormalRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "gamma") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new GammaRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "shiftedExponential") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ShiftedExponentialRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "shiftedRayleigh") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ShiftedRayleighRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "exponential") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ExponentialRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "rayleigh") == 0) {
			cout << "Random variable with tag " << tag << "cannot be created with only mean/stdv." << endl;
			return TCL_ERROR;
		}
		else if (strcmp(argv[2], "uniform") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new UniformRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "beta") == 0) {
			cout << "ERROR:: 'Beta' type random variable: use parameters to create!\n";
			return TCL_ERROR;
		}
		else if (strcmp(argv[2], "type1LargestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type1LargestValueRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "type1SmallestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type1SmallestValueRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "type2LargestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type2LargestValueRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "type3SmallestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type3SmallestValueRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "chiSquare") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ChiSquareRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "gumbel") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new GumbelRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "weibull") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new WeibullRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "laplace") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new LaplaceRV(tag, mean, stdv);
			}
		}
		else if (strcmp(argv[2], "pareto") == 0) {
			cout << "Random variable with tag " << tag << "cannot be created with only mean/stdv." << endl;
			return TCL_ERROR;
		}
		else {
			cout << "ERROR: unrecognized type of random variable number " << tag << endl;
			return TCL_ERROR;
		}

		if (theRandomVariable == 0) {
			cout << "ERROR: could not create random variable number " << tag << endl;
			return TCL_ERROR;
		}
	}
	else if (numberOfArguments == 6) {   // (Use mean/stdv AND startPt)

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[3], &mean) != TCL_OK) {
			cout << "ERROR: invalid input: mean \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &stdv) != TCL_OK) {
			cout << "ERROR: invalid input: stdv \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[5], &startPt) != TCL_OK) {
			cout << "ERROR: invalid input: startPt \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
		if (strcmp(argv[2], "normal") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new NormalRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "lognormal") == 0) {
			if (mean == 0.0 || stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new LognormalRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "gamma") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new GammaRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "shiftedExponential") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ShiftedExponentialRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "shiftedRayleigh") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ShiftedRayleighRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "exponential") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ExponentialRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "rayleigh") == 0) {
			cout << "Random variable with tag " << tag << "cannot be created with only mean/stdv." << endl;
		}
		else if (strcmp(argv[2], "uniform") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new UniformRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "beta") == 0) {
			cout << "ERROR:: 'Beta' type random variable: use parameters to create!\n";
			return TCL_ERROR;
		}
		else if (strcmp(argv[2], "type1LargestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type1LargestValueRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "type1SmallestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type1SmallestValueRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "type2LargestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type2LargestValueRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "type3SmallestValue") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new Type3SmallestValueRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "chiSquare") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new ChiSquareRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "gumbel") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new GumbelRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "weibull") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new WeibullRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "laplace") == 0) {
			if (stdv <= 0.0) {
				cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				return TCL_ERROR;
			}
			else {
				theRandomVariable = new LaplaceRV(tag, mean, stdv, startPt);
			}
		}
		else if (strcmp(argv[2], "pareto") == 0) {
			cout << "Random variable with tag " << tag << "cannot be created with only mean/stdv." << endl;
		}
		else {
			cout << "ERROR: unrecognized type of random variable number " << tag << endl;
			return TCL_ERROR;
		}

		if (theRandomVariable == 0) {
			cout << "ERROR: could not create random variable number " << tag << endl;
			return TCL_ERROR;
		}
	}
	else if (numberOfArguments == 7) {  // (Use parameters WITHOUT startPt)

		// GET INPUT PARAMETER (double)
		 if (Tcl_GetDouble(interp, argv[3], &parameter1) != TCL_OK) {
			 cout << "ERROR: invalid input: parameter1 \n";
			 return TCL_ERROR;
		 }

		 // GET INPUT PARAMETER (double)
		 if (Tcl_GetDouble(interp, argv[4], &parameter2) != TCL_OK) {
			 cout << "ERROR: invalid input: parameter2 \n";
			 return TCL_ERROR;
		 }

		 // GET INPUT PARAMETER (double)
		 if (Tcl_GetDouble(interp, argv[5], &parameter3) != TCL_OK) {
			 cout << "ERROR: invalid input: parameter3 \n";
			 return TCL_ERROR;
		 }

		 // GET INPUT PARAMETER (double)
		 if (Tcl_GetDouble(interp, argv[6], &parameter4) != TCL_OK) {
			 cout << "ERROR: invalid input: parameter4 \n";
			 return TCL_ERROR;
		 }

		 // GET INPUT PARAMETER (string) AND CREATE THE OBJECT
		 if (strcmp(argv[2], "normal") == 0) {
			 if (parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new NormalRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "lognormal") == 0) {

			 theRandomVariable = new LognormalRV(tag, parameter1, parameter2, parameter3, parameter4);

		 }
		 else if (strcmp(argv[2], "gamma") == 0) {
			 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new GammaRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "shiftedExponential") == 0) {
			 if (parameter1 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new ShiftedExponentialRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "shiftedRayleigh") == 0) {
			 if (parameter1 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new ShiftedRayleighRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "exponential") == 0) {
			 if (parameter1 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new ExponentialRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "rayleigh") == 0) {
			 if (parameter1 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new RayleighRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "uniform") == 0) {
			 if (parameter1 >= parameter2) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new UniformRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "beta") == 0) {
			 if (parameter1 >= parameter2 || parameter3 <= 0.0 || parameter4 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new BetaRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "type1LargestValue") == 0) {
			 if (parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new Type1LargestValueRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "type1SmallestValue") == 0) {
			 if (parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new Type1SmallestValueRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "type2LargestValue") == 0) {
			 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new Type2LargestValueRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "type3SmallestValue") == 0) {
			 if (parameter2 <= 0.0 || parameter3 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new Type3SmallestValueRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "chiSquare") == 0) {
			 if (parameter1 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new ChiSquareRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "gumbel") == 0) {
			 if (parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new GumbelRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "weibull") == 0) {
			 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new WeibullRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "laplace") == 0) {
			 if (parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new LaplaceRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }
		 else if (strcmp(argv[2], "pareto") == 0) {
			 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
				 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
			 else {
				 theRandomVariable = new ParetoRV(tag, parameter1, parameter2, parameter3, parameter4);
			 }
		 }

		 else if (numberOfArguments == 8) {  // (Use parameters AND startPt)

			 // GET INPUT PARAMETER (double)
			 if (Tcl_GetDouble(interp, argv[3], &parameter1) != TCL_OK) {
				 cout << "ERROR: invalid input: parameter1 \n";
				 return TCL_ERROR;
			 }

			 // GET INPUT PARAMETER (double)
			 if (Tcl_GetDouble(interp, argv[4], &parameter2) != TCL_OK) {
				 cout << "ERROR: invalid input: parameter2 \n";
				 return TCL_ERROR;
			 }

			 // GET INPUT PARAMETER (double)
			 if (Tcl_GetDouble(interp, argv[5], &parameter3) != TCL_OK) {
				 cout << "ERROR: invalid input: parameter3 \n";
				 return TCL_ERROR;
			 }

			 // GET INPUT PARAMETER (double)
			 if (Tcl_GetDouble(interp, argv[6], &parameter4) != TCL_OK) {
				 cout << "ERROR: invalid input: parameter4 \n";
				 return TCL_ERROR;
			 }

			 // GET INPUT PARAMETER (double)
			 if (Tcl_GetDouble(interp, argv[7], &startPt) != TCL_OK) {
				 cout << "ERROR: invalid input: startPt \n";
				 return TCL_ERROR;
			 }

			 // GET INPUT PARAMETER (string) AND CREATE THE OBJECT
			 if (strcmp(argv[2], "normal") == 0) {
				 if (parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new NormalRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "lognormal") == 0) {
				 //		  if ( parameter2 <= 0.0 ) { Now assume that this indicates negative lognormal
				 //			  cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
				 //			  return TCL_ERROR;
				 //		  }
				 //		  else  {
				 theRandomVariable = new LognormalRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 //		  }
			 }
			 else if (strcmp(argv[2], "gamma") == 0) {
				 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new GammaRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "shiftedExponential") == 0) {
				 if (parameter1 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new ShiftedExponentialRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "shiftedRayleigh") == 0) {
				 if (parameter1 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new ShiftedRayleighRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "exponential") == 0) {
				 if (parameter1 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new ExponentialRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "rayleigh") == 0) {
				 if (parameter1 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new RayleighRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "uniform") == 0) {
				 if (parameter1 >= parameter2) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new UniformRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "beta") == 0) {
				 if (parameter1 >= parameter2 || parameter3 <= 0.0 || parameter4 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new BetaRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "type1LargestValue") == 0) {
				 if (parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new Type1LargestValueRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "type1SmallestValue") == 0) {
				 if (parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new Type1SmallestValueRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "type2LargestValue") == 0) {
				 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new Type2LargestValueRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "type3SmallestValue") == 0) {
				 if (parameter2 <= 0.0 || parameter3 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new Type3SmallestValueRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "chiSquare") == 0) {
				 if (parameter1 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new ChiSquareRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "gumbel") == 0) {
				 if (parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new GumbelRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "weibull") == 0) {
				 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new WeibullRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "laplace") == 0) {
				 if (parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new LaplaceRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }
			 else if (strcmp(argv[2], "pareto") == 0) {
				 if (parameter1 <= 0.0 || parameter2 <= 0.0) {
					 cout << "ERROR: Invalid parameter input to random variable number " << tag << endl;
					 return TCL_ERROR;
				 }
				 else {
					 theRandomVariable = new ParetoRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);
				 }
			 }

			 // ------------- adding ---------
			 else if (strcmp(argv[2], "truncatedNormal") == 0) {
				 theRandomVariable = new TruncatedNormalRV(tag, parameter1, parameter2, parameter3, parameter4, startPt);

			 }


			 else {
				 cout << "ERROR: unrecognized type of random variable number " << tag << endl;
				 return TCL_ERROR;
			 }

			 if (theRandomVariable == 0) {
				 cout << "ERROR: could not create random variable number " << tag << endl;
				 return TCL_ERROR;
			 }
		 }
	 }
	 // ADD THE OBJECT TO THE DOMAIN
	 if (theReliabilityDomain->addRandomVariable(theRandomVariable) == false) {
		 cout << "ERROR: failed to add random variable to the domain (wrong number of arguments?)\n";
		 cout << "random variable: " << tag << endl;
		 delete theRandomVariable; // otherwise memory leak
		 return TCL_ERROR;
	 }
	 
	return TCL_OK;
}
	
//////////////////////////////////////////

int
TclReliabilityModelBuilder_getMean(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
	int tag;
	RandomVariable *rv;
	if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
		cout << "ERROR: Invalid random variable number tag to getMean command." << endl;
		return TCL_ERROR;
	}
	ReliabilityDomain *theReliabilityDomain=0;
	rv = theReliabilityDomain->getRandomVariablePtr(tag);
	if (rv == 0) {
		cout << "ERROR: Invalid tag number to getMean command. " << endl;
		return TCL_ERROR;
	}
	cout << "Mean of random variable number " << tag << ": " << rv->getMean() << endl;

	return TCL_OK;
}




//////////////////////////////////////////////////////////////////

int
TclReliabilityModelBuilder_getStdv(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
	int tag;
	RandomVariable *rv;
	if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
		cout << "ERROR: Invalid random variable number tag to getStdv command." << endl;
		return TCL_ERROR;
	}
	ReliabilityDomain *theReliabilityDomain=0;
	rv = theReliabilityDomain->getRandomVariablePtr(tag);
	if (rv == 0) {
		cout << "ERROR: Invalid tag number to getStdv command. " << endl;
		return TCL_ERROR;
	}
	cout << "Standard deviation of random variable number " << tag << ": " << rv->getStdv() << endl;

	return TCL_OK;
}




//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addCorrelate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	if (argc != 4) {
		cout << "ERROR: Wrong number of arguments to correlate command." << endl;
		return TCL_ERROR;
	}

	CorrelationCoefficient *theCorrelationCoefficient = 0;
	int tag;
	int rv1;
	int rv2;
	double correlationValue;


	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[1], &rv1) != TCL_OK) {
		cout << "ERROR: invalid input: rv1 \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[2], &rv2) != TCL_OK) {
		cout << "ERROR: invalid input: rv2 \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (double)
	if (Tcl_GetDouble(interp, argv[3], &correlationValue) != TCL_OK) {
		cout << "ERROR: invalid input: correlationValue \n";
		return TCL_ERROR;
	}
	
	// CREATE THE OBJECT
	tag = theReliabilityDomain->getNumberOfCorrelationCoefficients();
	theCorrelationCoefficient = new CorrelationCoefficient(tag + 1, rv1, rv2, correlationValue);

	if (theCorrelationCoefficient == 0) {
		cout << "ERROR: ran out of memory creating correlation coefficient \n";
		cout << "correlation coefficient: " << tag << endl;
		return TCL_ERROR;
	}

	// ADD THE OBJECT TO THE DOMAIN
	if (theReliabilityDomain->addCorrelationCoefficient(theCorrelationCoefficient) == false) {
		cout << "ERROR: failed to add correlation coefficient to the domain\n";
		cout << "correlation coefficient: " << tag << endl;
		delete theCorrelationCoefficient; // otherwise memory leak
		return TCL_ERROR;
	}

	return TCL_OK;
}
//////////////////////////////////////////

int
TclReliabilityModelBuilder_correlateGroup(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	int firstRV, lastRV;
	double correlationValue;

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[1], &firstRV) != TCL_OK) {
		cout << "ERROR: invalid input: firstRV \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[2], &lastRV) != TCL_OK) {
		cout << "ERROR: invalid input: lastRV \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (double)
	if (Tcl_GetDouble(interp, argv[3], &correlationValue) != TCL_OK) {
		cout << "ERROR: invalid input: correlationValue \n";
		return TCL_ERROR;
	}

	// Assume that previos corr. coeffs. have been added in order
	char theCorrelateCommand[50];
	for (int i = firstRV; i <= lastRV; i++) {
		for (int j = i + 1; j <= lastRV; j++) {
			//sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
			Tcl_Eval(interp, theCorrelateCommand);
		}
	}

	return TCL_OK;
}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_correlationStructure(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	int firstRV, lastRV;
	double theta, correlationValue;
	char theCorrelateCommand[50];

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[2], &firstRV) != TCL_OK) {
		cout << "ERROR: invalid input: firstRV \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[3], &lastRV) != TCL_OK) {
		cout << "ERROR: invalid input: lastRV \n";
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (double)
	if (Tcl_GetDouble(interp, argv[4], &theta) != TCL_OK) {
		cout << "ERROR: invalid input: theta \n";
		return TCL_ERROR;
	}


	// Create appropriate correlation coefficients
	if (strcmp(argv[1], "homogeneous1") == 0) {
		for (int i = firstRV; i <= lastRV; i++) {
			for (int j = i + 1; j <= lastRV; j++) {
				correlationValue = exp(-abs(i - j) / theta);
			//	sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
				Tcl_Eval(interp, theCorrelateCommand);
			}
		}
	}
	else if (strcmp(argv[1], "homogeneous2") == 0) {
		for (int i = firstRV; i <= lastRV; i++) {
			for (int j = i + 1; j <= lastRV; j++) {
				correlationValue = exp(-pow((i - j) / theta, 2.0));
		//		sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
				Tcl_Eval(interp, theCorrelateCommand);
			}
		}
	}
	else if (strcmp(argv[1], "homogeneous3") == 0) {
		for (int i = firstRV; i <= lastRV; i++) {
			for (int j = i + 1; j <= lastRV; j++) {
				correlationValue = 1.0 / (theta*(i - j)*(i - j));
			//	sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
				Tcl_Eval(interp, theCorrelateCommand);
			}
		}
	}
	else if (strcmp(argv[1], "homogeneous4") == 0) {
		for (int i = firstRV; i <= lastRV; i++) {
			for (int j = i + 1; j <= lastRV; j++) {
				if (abs(i - j) < theta) {
					correlationValue = 1.0 - (abs(i - j) / theta);
			//		sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
					Tcl_Eval(interp, theCorrelateCommand);
				}
			}
		}
	}
	else if (strcmp(argv[1], "vectorProduct") == 0) {

		// Open file where the vector is given
		
		

		// Give a warning if the number of elements of the vector is
		// different from the number of random variables being correlated
		
		

		for (int i = firstRV; i <= lastRV; i++) {
			for (int j = i + 1; j <= lastRV; j++) {
			//	correlationValue = theta * theVector(i - firstRV + 1) * theVector(j - firstRV + 1);
			//	sprintf(theCorrelateCommand, "correlate %d %d %10.5f", i, j, correlationValue);
				Tcl_Eval(interp, theCorrelateCommand);
			}
		}
	}
	else {
		cout << "ERROR: Invalid type of correlation structure. " << endl;
		return TCL_ERROR;
	}

	return TCL_OK;
}

//////////////////////////////////////////////////////////////////

int
TclReliabilityModelBuilder_addLimitState(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	LimitStateFunction *theLimitStateFunction = 0;
	int tag;

	if (theGFunEvaluator != 0) {
		cout << "ERROR: A limit-state function should not be created after the GFunEvaluator has been instantiated." << endl;
		return TCL_ERROR;
	}

	// GET INPUT PARAMETER (integer)
	if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
		cout << "ERROR: invalid input: tag \n";
		return TCL_ERROR;
	}

	// CREATE THE OBJECT (passing on argv[2])
	theLimitStateFunction = new LimitStateFunction(tag, argv[2]);
	if (theLimitStateFunction == 0) {
		cout << "ERROR: ran out of memory creating limit-state function \n";
		cout << "limit-state function: " << tag << endl;
		return TCL_ERROR;
	}

	// ADD THE OBJECT TO THE DOMAIN
	if (theReliabilityDomain->addLimitStateFunction(theLimitStateFunction) == false) {
		cout << "ERROR: failed to add limit-state function to the domain\n";
		cout << "limit-state function: " << tag << endl;
		delete theLimitStateFunction; // otherwise memory leak
		return TCL_ERROR;
	}
	
	return TCL_OK;
}

int
TclReliabilityModelBuilder_addProbabilityTransformation(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theProbabilityTransformation != 0) {
		delete theProbabilityTransformation;
		theProbabilityTransformation = 0;
	}


	// Check number of arguments
	if (argc != 2 && argc != 4) {
		cout << "ERROR: Wrong number of arguments to probability transformation." << endl;
		return TCL_ERROR;
	}


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[1], "Nataf") == 0) {

		int printFlag = 0;

		if (argc > 2) {
			if (strcmp(argv[2], "-print") == 0) {

				if (Tcl_GetInt(interp, argv[3], &printFlag) != TCL_OK) {
					cout << "ERROR: invalid input: printFlag to Nataf transformation \n";
					return TCL_ERROR;
				}
			}
		}

		theProbabilityTransformation = new NatafProbabilityTransformation(theReliabilityDomain, printFlag);
	}
	else {
		cout << "ERROR: unrecognized type of ProbabilityTransformation \n";
		return TCL_ERROR;
	}

	if (theProbabilityTransformation == 0) {
		cout << "ERROR: could not create theProbabilityTransformation \n";
		return TCL_ERROR;
	}
	
	return TCL_OK;
}
//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addReliabilityConvergenceCheck(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theReliabilityConvergenceCheck != 0) {
		delete theReliabilityConvergenceCheck;
		theReliabilityConvergenceCheck = 0;
	}


	if (argc < 2) {
		cout << "ERROR: Wrong number of arguments to reliability convergence check." << endl;
		return TCL_ERROR;
	}

	// Initial declarations
	int argvCounter = 1;


	if (strcmp(argv[argvCounter], "Standard") == 0) {
		argvCounter++;

		double e1 = 1.0e-3;
		double e2 = 1.0e-3;
		double scaleValue = 0.0;
		int print = 1;

		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-e1") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &e1) != TCL_OK) {
					cout << "ERROR: invalid input: e1 \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-e2") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &e2) != TCL_OK) {
					cout << "ERROR: invalid input: e2 \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-print") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &print) != TCL_OK) {
					cout << "ERROR: invalid input: print \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-scaleValue") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &scaleValue) != TCL_OK) {
					cout << "ERROR: invalid input: scaleValue \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to standard reliability convergence check. " << endl;
				return TCL_ERROR;
			}
		}
		theReliabilityConvergenceCheck = new StandardReliabilityConvergenceCheck(e1, e2, scaleValue, print);
	}
	else if (strcmp(argv[argvCounter], "OptimalityCondition") == 0) {
		argvCounter++;

		double e1 = 1.0e-3;
		double e2 = 1.0e-3;
		double scaleValue = 0.0;
		int print = 1;

		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-e1") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &e1) != TCL_OK) {
					cout << "ERROR: invalid input: e1 \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-e2") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &e2) != TCL_OK) {
					cout << "ERROR: invalid input: e2 \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-print") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &print) != TCL_OK) {
					cout << "ERROR: invalid input: print \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-scaleValue") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &scaleValue) != TCL_OK) {
					cout << "ERROR: invalid input: scaleValue \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to standard reliability convergence check. " << endl;
				return TCL_ERROR;
			}
		}
		theReliabilityConvergenceCheck = new OptimalityConditionReliabilityConvergenceCheck(e1, e2, scaleValue, print);
	}
	else {
		cout << "ERROR: unrecognized type of ReliabilityConvergenceCheck \n";
		return TCL_ERROR;
	}

	if (theReliabilityConvergenceCheck == 0) {
		cout << "ERROR: could not create theReliabilityConvergenceCheck \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}
//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addgFunEvaluator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theGFunEvaluator != 0) {
		delete theGFunEvaluator;
		theGFunEvaluator = 0;
	}

	int nsteps = 0;
	double dt = 0;
	//char outPutFile[25] = "";
	char * tclFileName = 0;
	if (strcmp(argv[1], "OpenSees") == 0) {

		if (strcmp(argv[2], "Timeinvariant") == 0) {
			if (argc < 4) {
				cout << "ERROR: Too few arguments to gFunEvaluator" << endl;
				return TCL_ERROR;
			}

			if (strcmp(argv[3], "-file") == 0) {

				// Try to open the file to make sure it exists
				double dt = 0.0;
				ifstream inputFile(argv[4], ios::in);
				if (inputFile.fail()) {
					cout << "File " << *argv[4] << " could not be opened. " << endl;
					return TCL_ERROR;
				}
				inputFile.close();

				if (argc == 6) {
					if (Tcl_GetDouble(interp, argv[5], &dt) != TCL_OK) {
						cout << "ERROR: invalid input: dt for OpenSees GFunEvaluator \n";
						return TCL_ERROR;
					}
				}
				theGFunEvaluator = new OpenSeesGFunEvaluator(
					interp, theReliabilityDomain, argv[4], dt);
			}
		}
		else if (strcmp(argv[2], "Timevariant") == 0)
		{
			double dt;
			if (strcmp(argv[3], "-analyze") == 0) {

				if (Tcl_GetInt(interp, argv[4], &nsteps) != TCL_OK) {
					cout << "ERROR: invalid input: numbersteps \n";
					return TCL_ERROR;
				}
			}// if


			if (Tcl_GetDouble(interp, argv[5], &dt) != TCL_OK) {
				cout << "ERROR: invalid input: dt \n";
				return TCL_ERROR;
			}

			if (strcmp(argv[6], "-file") == 0) {

				// Try to open the file to make sure it exists
				double dt = 0.0;
				ifstream inputFile(argv[7], ios::in);
				if (inputFile.fail()) {
					cout << "File " << *argv[8] << " could not be opened. " << endl;
					return TCL_ERROR;
				}
				inputFile.close();
			}
			if (argc == 7) {
				if (Tcl_GetDouble(interp, argv[8], &dt) != TCL_OK) {
					cout << "ERROR: invalid input: dt for OpenSees GFunEvaluator \n";
					return TCL_ERROR;
				}
			}
			theGFunEvaluator = new OpenSeesGFunEvaluator(
				interp, theReliabilityDomain, nsteps, dt, argv[7]);

		}
		else
		{
			cout << "ERROR: invalid input for OpenSees GFunEvaluator";
		}
		// There are several alternatives for this command:
		// gFunEvaluator  OpenSees  -file <filename> <dT (optional for outcrossing only)> 
		// gFunEvaluator  OpenSees  -runToMaxTimeInGFun
		// gFunEvaluator  OpenSees  -analyze <numSteps> <dt(optional)>
	}
		else if (strcmp(argv[1], "Basic") == 0) {
			theGFunEvaluator = new BasicGFunEvaluator(interp, theReliabilityDomain);
		}
		/*else {
			cout << "ERROR: unrecognized type of GFunEvaluator \n";
			return TCL_ERROR;
		}
	*/
	
	if (theGFunEvaluator == 0) {
		cout << "ERROR: could not create the theGFunEvaluator \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}
//////////////////////////////////////////////////////////////////

int
TclReliabilityModelBuilder_addGradGEvaluator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theGradGEvaluator != 0) {
		delete theGradGEvaluator;
		theGradGEvaluator = 0;
	}


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[1], "FiniteDifference") == 0) {

		double perturbationFactor = 1000.0;
		bool doGradientCheck = false;

		// Check that the necessary ingredients are present
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a FiniteDifferenceGradGEvaluator can be created" << endl;
			return TCL_ERROR;
		}

		// Possibly read perturbation factor
		if (argc > 2) {
			int numExtras;
			if (argc == 3 || argc == 4) {
				numExtras = 1;
			}
			else if (argc == 5) {
				numExtras = 2;
			}
			else {
				cout << "ERROR: Wrong number of arguments to gradGEvaluator. " << endl;
				return TCL_ERROR;
			}

			int counter = 2;

			for (int i = 1; i <= numExtras; i++) {

				if (strcmp(argv[counter], "-pert") == 0) {
					counter++;

					if (Tcl_GetDouble(interp, argv[counter], &perturbationFactor) != TCL_OK) {
						cout << "ERROR: invalid input: perturbationFactor \n";
						return TCL_ERROR;
					}
					counter++;
				}
				else if (strcmp(argv[counter], "-check") == 0) {
					counter++;
					doGradientCheck = true;
				}
				else {
					cout << "ERROR: Error in input to gradGEvaluator. " << endl;
					return TCL_ERROR;
				}
			}
		}

		theGradGEvaluator = new FiniteDifferenceGradGEvaluator(theGFunEvaluator, theReliabilityDomain, interp, perturbationFactor, doGradientCheck, false);
	}
	/*else if (strcmp(argv[1], "OpenSees") == 0) {

		bool doGradientCheck = false;

		//Quan Apr. 2006	
		if (theSensitivityAlgorithm == 0) {
			cout << "Warning:Need a DDM sensitivity algorithm before a OpenSees sensitivity evaluator can be created" << endl;
			//		return TCL_ERROR;
		}

		if (argc == 2) {
			// Do nothing
		}
		else if (argc == 3) {
			if (strcmp(argv[2], "-check") == 0) {
				doGradientCheck = true;
			}
		}
		else {
			cout << "ERROR: Wrong number of arguments to gradGEvaluator. " << endl;
			return TCL_ERROR;
		}

		theGradGEvaluator = new OpenSeesGradGEvaluator(interp, theReliabilityDomain, doGradientCheck);
	}
	else {
		cout << "ERROR: unrecognized type of GradGEvaluator \n";
		return TCL_ERROR;
	}
	*/
	if (theGradGEvaluator == 0) {
		cout << "ERROR: could not create theGradGEvaluator \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}

////////////////////////////////////////////

int
TclReliabilityModelBuilder_addSearchDirection(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theSearchDirection != 0) {
		delete theSearchDirection;
		theSearchDirection = 0;
	}


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[1], "iHLRF") == 0) {

		if (argc != 2) {
			cout << "ERROR: Wrong number of arguments to iHLRF search direction. " << endl;
			return TCL_ERROR;
		}

		theSearchDirection = new HLRFSearchDirection();

		if (theSearchDirection == 0) {
			cout << "ERROR: could not create theSearchDirection \n";
			return TCL_ERROR;
		}


	}
	else if (strcmp(argv[1], "PolakHe") == 0) {

		double gamma = 1.0;
		double delta = 1.0;

		int argvCounter = 2;
		while (argc > argvCounter) {
			if (strcmp(argv[argvCounter], "-gamma") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &gamma) != TCL_OK) {
					cout << "ERROR: invalid input: gamma for Polak-He algorithm" << endl;
					return TCL_ERROR;
				}
				argvCounter++;

			}
			else if (strcmp(argv[argvCounter], "-delta") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &delta) != TCL_OK) {
					cout << "ERROR: invalid input: delta for Polak-He algorithm" << endl;
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to Polak-He algorithm." << endl;
				return TCL_ERROR;
			}
		}


		thePolakHeDualPurpose = new PolakHeSearchDirectionAndMeritFunction(gamma, delta);
		theSearchDirection = thePolakHeDualPurpose;
	}
	else if (strcmp(argv[1], "GradientProjection") == 0) {

		if (argc != 2) {
			cout << "ERROR: Wrong number of arguments to GradientProjection search direction. " << endl;
			return TCL_ERROR;
		}


		// Check that a step size rule has been created
		if (theStepSizeRule == 0) {
			cout << "Need theStepSizeRule before a GradientProjectionSearchDirection can be created" << endl;
			return TCL_ERROR;
		}

		// Check that a transformation has been created
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a GradientProjectionSearchDirection can be created" << endl;
			return TCL_ERROR;
		}

		// Check that a gfun evaluator has been created
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a GradientProjectionSearchDirection can be created" << endl;
			return TCL_ERROR;
		}

		// Check that a root-finding algorithm has been created
		if (theRootFindingAlgorithm == 0) {
			cout << "Need theRootFindingAlgorithm before a GradientProjectionSearchDirection can be created" << endl;
			return TCL_ERROR;
		}


		theSearchDirection = new GradientProjectionSearchDirection(theStepSizeRule,
			theProbabilityTransformation,
			theGFunEvaluator,
			theRootFindingAlgorithm);
	}
	
	else if (strcmp(argv[1], "SQP") == 0) {

		double c_bar = 200.0;
		double e_bar = 0.5;

		int argvCounter = 2;
		while (argc > argvCounter) {
			if (strcmp(argv[argvCounter], "-c_bar") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &c_bar) != TCL_OK) {
					cout << "ERROR: invalid input: c_bar for algorithm" << endl;
					return TCL_ERROR;
				}
				argvCounter++;

			}
			else if (strcmp(argv[argvCounter], "-e_bar") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &e_bar) != TCL_OK) {
					cout << "ERROR: invalid input: e_bar for SQP algorithm" << endl;
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to SQP algorithm." << endl;
				return TCL_ERROR;
			}
		}


		theSQPtriplePurpose = new SQPsearchDirectionMeritFunctionAndHessian(c_bar, e_bar);
		theSearchDirection = theSQPtriplePurpose;

		// Set default Hessian approximation in case user forgets
		theHessianApproximation = theSQPtriplePurpose;

		// Set the Hessian approximation in the search direction
		theSQPtriplePurpose->setHessianApproximation(theHessianApproximation);


	}
	else {
		cout << "ERROR: unrecognized type of SearchDirection \n";
		return TCL_ERROR;
	}
	
	if (theSearchDirection == 0) {
		cout << "ERROR: could not create theSearchDirection \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addMeritFunctionCheck(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	
	// In case this is a replacement
	if (theMeritFunctionCheck != 0) {
		delete theMeritFunctionCheck;
		theMeritFunctionCheck = 0;
	}

	int argvCounter = 1;

	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[argvCounter], "AdkZhang") == 0) {
		argvCounter++;

		double multi = 2.0;
		double add = 10.0;
		double factor = 0.0;

		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-multi") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &multi) != TCL_OK) {
					cout << "ERROR: invalid input: multi \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-add") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &add) != TCL_OK) {
					cout << "ERROR: invalid input: add \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-factor") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &factor) != TCL_OK) {
					cout << "ERROR: invalid input: factor \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to AdkZhang merit function check. " << endl;
				return TCL_ERROR;
			}
		}

		// Do a quick input check
		if (multi < 1.0 || add < 0.0) {
			cout << "ERROR: Invalid values of multi/add parameters to AdkZhang merit function check." << endl;
			return TCL_ERROR;
		}

		theMeritFunctionCheck = new AdkZhangMeritFunctionCheck(multi, add, factor);
	}
	else if (strcmp(argv[argvCounter], "PolakHe") == 0) {
		argvCounter++;

		// Check that the PolakHe search direction is already created
		if (thePolakHeDualPurpose == 0) {
			cout << "Need thePolakHeSearchDirection before a PolakHe merit function can be created" << endl;
			return TCL_ERROR;
		}
		double factor = 0.5;
		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-factor") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &factor) != TCL_OK) {
					cout << "ERROR: invalid input: factor \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to Polak He merit function check. " << endl;
				return TCL_ERROR;
			}
		}

		thePolakHeDualPurpose->setAlpha(factor);
		theMeritFunctionCheck = thePolakHeDualPurpose;

	}
	else if (strcmp(argv[argvCounter], "SQP") == 0) {
		argvCounter++;

		// Check that the SQP search direction is already created
		if (theSQPtriplePurpose == 0) {
			cout << "Need theSQPSearchDirection before a SQP merit function can be created" << endl;
			return TCL_ERROR;
		}

		double factor = 0.5;
		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-factor") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &factor) != TCL_OK) {
					cout << "ERROR: invalid input: factor \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to SQP merit function check. " << endl;
				return TCL_ERROR;
			}
		}

		theSQPtriplePurpose->setAlpha(factor);
		theMeritFunctionCheck = theSQPtriplePurpose;

	}
	else {
		cout << "ERROR: unrecognized type of MeritFunctionCheck \n";
		return TCL_ERROR;
	}
	
	if (theMeritFunctionCheck == 0) {
		cout << "ERROR: could not create theMeritFunctionCheck \n";
		return TCL_ERROR;
	}
	
	return TCL_OK;
}

////////////////////////////////////
int
TclReliabilityModelBuilder_addStepSizeRule(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theStepSizeRule != 0) {
		delete theStepSizeRule;
		theStepSizeRule = 0;
	}


	// Initial declarations
	int argvCounter = 1;

	
	if (strcmp(argv[argvCounter], "Armijo") == 0) {
		argvCounter++;

		// Check that the necessary ingredients are present
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before an ArmijoStepSizeRule can be created" << endl;
			return TCL_ERROR;
		}
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a ArmijoStepSizeRule can be created" << endl;
			return TCL_ERROR;
		}
		if (theMeritFunctionCheck == 0) {
			cout << "Need theMeritFunctionCheck before a ArmijoStepSizeRule can be created" << endl;
			return TCL_ERROR;
		}

		// Get input parameters
		double base = 0.5;
		int maxNumReductions = 10;

		double b0 = 1.0;
		int numberOfShortSteps = 2;

		double radius = 50.0;
		double surfaceDistance = 0.1;
		double evolution = 0.5;

		int printFlag = 0;

		while (argvCounter < argc) {


			if (strcmp(argv[argvCounter], "-print") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &printFlag) != TCL_OK) {
					cout << "ERROR: invalid input: printFlag \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-maxNum") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &maxNumReductions) != TCL_OK) {
					cout << "ERROR: invalid input: maxNumReductions \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-base") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &base) != TCL_OK) {
					cout << "ERROR: invalid input: base \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-initial") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &b0) != TCL_OK) {
					cout << "ERROR: invalid input: b0 \n";
					return TCL_ERROR;
				}
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &numberOfShortSteps) != TCL_OK) {
					cout << "ERROR: invalid input: numberOfShortSteps \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-sphere") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &radius) != TCL_OK) {
					cout << "ERROR: invalid input: radius \n";
					return TCL_ERROR;
				}
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &surfaceDistance) != TCL_OK) {
					cout << "ERROR: invalid input: surfaceDistance \n";
					return TCL_ERROR;
				}
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &evolution) != TCL_OK) {
					cout << "ERROR: invalid input: evolution \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to Armijo rule. " << endl;
				return TCL_ERROR;
			}
		}

		theStepSizeRule = new ArmijoStepSizeRule(theGFunEvaluator,
			theProbabilityTransformation,
			theMeritFunctionCheck,
			theRootFindingAlgorithm,
			base,
			maxNumReductions,
			b0,
			numberOfShortSteps,
			radius,
			surfaceDistance,
			evolution,
			printFlag);


	}
	
	else if (strcmp(argv[argvCounter], "Fixed") == 0) {
		argvCounter++;

		double stepSize = 1.0;

		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-stepSize") == 0) {
				argvCounter++;

				if (Tcl_GetDouble(interp, argv[argvCounter], &stepSize) != TCL_OK) {
					cout << "ERROR: Invalid step size input to Fixed step size rule." << endl;
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to Fixed step size rule. " << endl;
				return TCL_ERROR;
			}

		}


		theStepSizeRule = new FixedStepSizeRule(stepSize);
	}
	
	else {
		cout << "ERROR: unrecognized type of StepSizeRule \n";
		return TCL_ERROR;
	}
	
	
	if (theStepSizeRule == 0) {
		cout << "ERROR: could not create theStepSizeRule \n";
		return TCL_ERROR;
	}

	return TCL_OK;
}

////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addStartPoint(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	
	// In case this is a replacement
	if (theStartPoint != 0) {
		delete theStartPoint;
		theStartPoint = 0;
	}


	// Check that there are enough arguments
	if (argc < 2) {
		cout << "ERROR: Not enough arguments to theStartPoint. " << endl;
		return TCL_ERROR;
	}

	int nrv = theReliabilityDomain->getNumberOfRandomVariables();
	RandomVariable *aRandomVariable;


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[1], "Mean") == 0) {

		theStartPoint = new Vector(nrv);

		for (int i = 1; i <= nrv; i++)
		{
			aRandomVariable = theReliabilityDomain->getRandomVariablePtr(i);
			if (aRandomVariable == 0) {
				cout << "ERROR: when creating theStartPoint - could not find" << endl
					<< " random variable with tag #" << i << "." << endl;
				return TCL_ERROR;
			}
			(*theStartPoint)(i - 1) = aRandomVariable->getMean();
		}
	}
	else if (strcmp(argv[1], "Origin") == 0) {
		// This is the default option (at least from now on...)
		// Do nothing; theStartPoint==0 is the indication of this case
	}
	else if (strcmp(argv[1], "Given") == 0) {

		theStartPoint = new Vector(nrv);

		for (int i = 1; i <= nrv; i++)
		{
			aRandomVariable = theReliabilityDomain->getRandomVariablePtr(i);
			if (aRandomVariable == 0) {
				cout << "ERROR: when creating theStartPoint - could not find" << endl
					<< " random variable with tag #" << i << "." << endl;
				return TCL_ERROR;
			}
			(*theStartPoint)(i - 1) = aRandomVariable->getStartValue();
		}
	}
	//startPoint -file  fileName?   <-shiftStartPt>
	else if (strcmp(argv[1], "-file") == 0) {

		theStartPoint = new Vector(nrv);

		ifstream inputFile(argv[2], ios::in);
		if (inputFile.fail()) {
			cout << "File " << *argv[2] << " could not be opened. " << endl;
			return TCL_ERROR;
		}

		// Loop through file to see how many entries there are
		int numRVs = theReliabilityDomain->getNumberOfRandomVariables();
		double dummy;
		int numEntries = 0;
		while (inputFile >> dummy) {
			numEntries++;
		}

		// Close the file
		inputFile.close();

		if (numEntries == 0) {
			cout << "ERROR: No entries in the file read by startPoint!" << endl;
			return TCL_ERROR;
		}

		// Open it again, now being ready to store the results
		ifstream inputFile2(argv[2], ios::in);
		for (int i = 0; i < numEntries; i++) {
			inputFile2 >> (*theStartPoint)(i);
		}
		inputFile2.close();



		if (numEntries != numRVs) {
			if ((strcmp(argv[3], "-shiftStartPt") == 0) & (numEntries < numRVs)) {
				int i = numRVs - 1;
				for (int j = numEntries - 1; j >= 0; j--) {
					(*theStartPoint)(i) = (*theStartPoint)(j);

					i--;
				} //for
				for (; i >= 0; i--) (*theStartPoint)(i) = 0;
			}  //if
			else {
				cout << "ERROR: Wrong number of entries in the file read by startPoint." << endl;
				return TCL_ERROR;
			}
		}


	}
	else {
		cout << "ERROR: Invalid type of start point is given. " << endl;
		return TCL_ERROR;
	}

	// Check that the vector is of correct size
	if (theStartPoint == 0) {
		//		cout << "ERROR: Could not create the start point. " << endl;
		//		return TCL_ERROR;
	}
	else {
		if (theStartPoint->Size() != nrv) {
			cout << "ERROR: The size of the start point vector is NOT equal " << endl
				<< " to the number of random variables in the model! " << endl;
			return TCL_ERROR;
		}
	}

	return TCL_OK;
}

////////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addFindDesignPointAlgorithm(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	
	// In case this is a replacement
	if (theFindDesignPointAlgorithm != 0) {
		delete theFindDesignPointAlgorithm;
		theFindDesignPointAlgorithm = 0;
	}

	if (argc < 2) {
		cout << "ERROR: Wrong number of arguments to find design point algorithm." << endl;
		return TCL_ERROR;
	}

	int argvCounter = 1;

	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[argvCounter], "StepSearch") == 0) {
		argvCounter++;

		// Check that the necessary ingredients are present
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		if (theGradGEvaluator == 0) {
			cout << "Need theGradGEvaluator before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		if (theStepSizeRule == 0) {
			cout << "Need theStepSizeRule before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		if (theSearchDirection == 0) {
			cout << "Need theSearchDirection before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		//		if (theStartPoint == 0 ) {
		//			cout << "Need theStartPoint before a FindDesignPointAlgorithm can be created" << endl;
		//			return TCL_ERROR;
		//		}
		if (theReliabilityConvergenceCheck == 0) {
			cout << "Need theReliabilityConvergenceCheck before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}

		int printFlag = 0;
		char *fileNamePrint;
		fileNamePrint = new char[256];
		strcpy(fileNamePrint, "initialized");


		int maxNumIter = 100;
		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-maxNumIter") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &maxNumIter) != TCL_OK) {
					cout << "ERROR: invalid input: maxNumIter \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printAllPointsX") == 0) {
				argvCounter++;
				printFlag = 1;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printAllPointsY") == 0) {
				argvCounter++;
				printFlag = 2;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printDesignPointX") == 0) {
				argvCounter++;
				printFlag = 3;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printDesignPointY") == 0) {
				argvCounter++;
				printFlag = 4;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printCurrentPointX") == 0) {
				argvCounter++;
				printFlag = 5;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printCurrentPointY") == 0) {
				argvCounter++;
				printFlag = 6;
				strcpy(fileNamePrint, argv[argvCounter]);
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to SearchWithStepSizeAndStepDirection. " << endl;
				return TCL_ERROR;
			}
		}

		theFindDesignPointAlgorithm = new SearchWithStepSizeAndStepDirection(
			maxNumIter,
			theGFunEvaluator,
			theGradGEvaluator,
			theStepSizeRule,
			theSearchDirection,
			theProbabilityTransformation,
			theHessianApproximation,
			theReliabilityConvergenceCheck,
			printFlag,
			fileNamePrint,
			theStartPoint);

		delete[] fileNamePrint;

	}   //if stepSize
	/*
	// Quan SNOPT interface ----
	else if (strcmp(argv[argvCounter], "SNOPT") == 0) {
		argvCounter++;

		// Check that the necessary ingredients are present
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		if (theGradGEvaluator == 0) {
			cout << "Need theGradGEvaluator before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a FindDesignPointAlgorithm can be created" << endl;
			return TCL_ERROR;
		}
		
		int printFlag = 0;
		char *fileNamePrint1;
		fileNamePrint1 = new char[256];
		strcpy(fileNamePrint1, "initialized");


		int maxNumIter = 250;
		while (argvCounter < argc) {

			if (strcmp(argv[argvCounter], "-maxNumIter") == 0) {
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &maxNumIter) != TCL_OK) {
					cout << "ERROR: invalid input: maxNumIter \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printAllPointsX") == 0) {
				argvCounter++;
				printFlag = 1;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printAllPointsY") == 0) {
				argvCounter++;
				printFlag = 2;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printDesignPointX") == 0) {
				argvCounter++;
				printFlag = 3;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printDesignPointY") == 0) {
				argvCounter++;
				printFlag = 4;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printCurrentPointX") == 0) {
				argvCounter++;
				printFlag = 5;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-printCurrentPointY") == 0) {
				argvCounter++;
				printFlag = 6;
				strcpy(fileNamePrint1, argv[argvCounter]);
				argvCounter++;
			}
			else {
				cout << "ERROR: Invalid input to SearchWithStepSizeAndStepDirection. " << endl;
				return TCL_ERROR;
			}
		}

		char *ProbType = new char[256];
		strcpy(ProbType, "reliability");
		/*

		theSNOPT = new snoptProblem(
			maxNumIter,
			theGFunEvaluator,
			theGradGEvaluator,
			theProbabilityTransformation,
			printFlag,
			fileNamePrint1,
			theStartPoint,
			ProbType, theReliabilityDomain);

		theFindDesignPointAlgorithm = theSNOPT;  // is this transfer correct 

		delete[] fileNamePrint1;  // something wrong here

	}   //if SNOPT  ---- Quan



	else {
		cout << "ERROR: unrecognized type of FindDesignPointAlgorithm Algorithm \n";
		return TCL_ERROR;
	}
	
	if (theFindDesignPointAlgorithm == 0) {
		cout << "ERROR: could not create theFindDesignPointAlgorithm \n";
		return TCL_ERROR;
	}
	*/
	return TCL_OK;
}


//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runFORMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theFORMAnalysis != 0) {
		delete theFORMAnalysis;
		theFORMAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	// Check number of arguments
	if ((argc != 2) && (argc != 4)) {
		cout << "ERROR: Wrong number of input parameter to FORM analysis" << endl;
		return TCL_ERROR;
	}


	// Check for essential tools
	if (theFindDesignPointAlgorithm == 0) {
		cout << "Need theFindDesignPointAlgorithm before a FORMAnalysis can be created" << endl;
		return TCL_ERROR;
	}
	if (theProbabilityTransformation == 0) {
		cout << "Need theProbabilityTransformation before a FORMAnalysis can be created" << endl;
		return TCL_ERROR;
	}


	// Read input parameter(s)
	int relSensTag = 0;
	if (argc == 4) {
		if (strcmp(argv[2], "-relSens") == 0) {
			if (Tcl_GetInt(interp, argv[3], &relSensTag) != TCL_OK) {
				cout << "ERROR: invalid input: relSensTag \n";
				return TCL_ERROR;
			}
		}
		else {
			cout << "ERROR: Invalid input to FORMAnalysis." << endl;
			return TCL_ERROR;
		}
	}


	// Create the analysis object
	theFORMAnalysis
		= new FORMAnalysis(theReliabilityDomain,
			theFindDesignPointAlgorithm,
			theProbabilityTransformation,
			argv[1],
			relSensTag);


	// Check that it really was created
	if (theFORMAnalysis == 0) {
		cout << "ERROR: could not create theFORMAnalysis \n";
		return TCL_ERROR;
	}


	// Now run the analysis
	theFORMAnalysis->analyze();
	
	return TCL_OK;
}

/////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runFOSMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theFOSMAnalysis != 0) {
		delete theFOSMAnalysis;
		theFOSMAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	// Check number of arguments
	if (argc != 2) {
		cout << "ERROR: Wrong number of input parameter to FOSM analysis" << endl;
		return TCL_ERROR;
	}


	// Check for essential ingredients
	if (theGFunEvaluator == 0) {
		cout << "Need theGFunEvaluator before a FOSMAnalysis can be created" << endl;
		return TCL_ERROR;
	}
	if (theGradGEvaluator == 0) {
		cout << "Need theGradGEvaluator before a FOSMAnalysis can be created" << endl;
		return TCL_ERROR;
	}


	theFOSMAnalysis = new FOSMAnalysis(theReliabilityDomain,
		theGFunEvaluator,
		theGradGEvaluator,
		interp,
		argv[1]);

	if (theFOSMAnalysis == 0) {
		cout << "ERROR: could not create theFOSMAnalysis \n";
		return TCL_ERROR;
	}

	// Now run the analysis
	theFOSMAnalysis->analyze();
	
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runSORMAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	
	// In case this is a replacement
	if (theSORMAnalysis != 0) {
		delete theSORMAnalysis;
		theSORMAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	if (theFindCurvatures == 0) {
		cout << "Need theFindCurvatures before a SORMAnalysis can be created" << endl;
		return TCL_ERROR;
	}
	if (theFORMAnalysis == 0) {
		cout << "ERROR: The current SORM implementation requires a FORM analysis" << endl
			<< " to have been executed previously in the same session." << endl;
		return TCL_ERROR;
	}

	if (argc != 2) {
		cout << "ERROR: Wrong number of arguments to SORM analysis" << endl;
		return TCL_ERROR;
	}

	theSORMAnalysis
		= new SORMAnalysis(theReliabilityDomain, theFindCurvatures, argv[1]);

	if (theSORMAnalysis == 0) {
		cout << "ERROR: could not create theSORMAnalysis \n";
		return TCL_ERROR;
	}

	// Now run the analysis
	theSORMAnalysis->analyze();
	
	return TCL_OK;
}


//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addRootFinding(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theRootFindingAlgorithm != 0) {
		delete theRootFindingAlgorithm;
		theRootFindingAlgorithm = 0;
	}


	if (theGFunEvaluator == 0) {
		cout << "Need theGFunEvaluator before a root-finding algorithm can be created" << endl;
		return TCL_ERROR;
	}

	if (theProbabilityTransformation == 0) {
		cout << "Need theProbabilityTransformation before a root-finding algorithm can be created" << endl;
		return TCL_ERROR;
	}


	int maxIter = 50;
	double tol = 1.0e-3;
	double maxStepLength = 1.0;

	int argvCounter = 2;
	while (argc > argvCounter) {
		if (strcmp(argv[argvCounter], "-maxIter") == 0) {
			argvCounter++;

			if (Tcl_GetInt(interp, argv[argvCounter], &maxIter) != TCL_OK) {
				cout << "ERROR: invalid input: maxIter for projection" << endl;
				return TCL_ERROR;
			}
			argvCounter++;

		}
		else if (strcmp(argv[argvCounter], "-tol") == 0) {
			argvCounter++;

			if (Tcl_GetDouble(interp, argv[argvCounter], &tol) != TCL_OK) {
				cout << "ERROR: invalid input: tol factor \n";
				return TCL_ERROR;
			}
			argvCounter++;

		}
		else if (strcmp(argv[argvCounter], "-maxStepLength") == 0) {
			argvCounter++;


			if (Tcl_GetDouble(interp, argv[argvCounter], &maxStepLength) != TCL_OK) {
				cout << "ERROR: invalid input: maxStepLength factor \n";
				return TCL_ERROR;
			}
			argvCounter++;
		}
		else {
			cout << "ERROR: Invalid input to projection algorithm. " << endl;
			return TCL_ERROR;
		}

	}


	if (strcmp(argv[1], "Secant") == 0) {

		theRootFindingAlgorithm = new SecantRootFinding(
			theReliabilityDomain,
			theProbabilityTransformation,
			theGFunEvaluator,
			maxIter,
			tol,
			maxStepLength);

	}
	else {
		cout << "ERROR: unrecognized type of root-finding algorithm \n";
		return TCL_ERROR;
	}

	if (theRootFindingAlgorithm == 0) {
		cout << "ERROR: could not create root-finding algorithm \n";
		return TCL_ERROR;
	}
	

	return TCL_OK;
}

/////////////////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addRandomNumberGenerator(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theRandomNumberGenerator != 0) {
		delete theRandomNumberGenerator;
		theRandomNumberGenerator = 0;
	};


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[1], "CStdLib") == 0) {
		theRandomNumberGenerator = new CStdLibRandGenerator();
	}

	else if ((strcmp(argv[1], "MersenneTwister") == 0) || (strcmp(argv[1], "MT") == 0)) {
		theRandomNumberGenerator = new MersenneTwister();
	}

	else {
		cout << "ERROR: unrecognized type of RandomNumberGenerator \n";
		return TCL_ERROR;
	};

	if (theRandomNumberGenerator == 0) {
		cout << "ERROR: could not create theRandomNumberGenerator \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runImportanceSamplingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theImportanceSamplingAnalysis != 0) {
		delete theImportanceSamplingAnalysis;
		theImportanceSamplingAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	// Check for essential tools
	if (theProbabilityTransformation == 0) {
		cout << "Need theProbabilityTransformation before a SimulationAnalyis can be created" << endl;
		return TCL_ERROR;
	}
	if (theGFunEvaluator == 0) {
		cout << "Need theGFunEvaluator before a SimulationAnalyis can be created" << endl;
		return TCL_ERROR;
	}
	if (theRandomNumberGenerator == 0) {
		cout << "Need theRandomNumberGenerator before a SimulationAnalyis can be created" << endl;
		return TCL_ERROR;
	}


	// The following switches are available (default values are provided)
	// (The sampling is performed around theStartPoint, except 
	// for response statistics sampling; then the mean is used together
	// with unit sampling variance.)
	//
	//     -type  failureProbability (1)......... this is the default
	//     -type  responseStatistics (2)
	//     -type  saveGvalues (3)
	//
	//     -variance 1.0  ....................... this is the default
	//
	//     -maxNum 1000  ........................ this is the default
	//
	//     -targetCOV 0.05  ..................... this is the default
	//
	//     -print 0   (print nothing) ........... this is the default
	//     -print 1   (print to screen)
	//     -print 2   (print to restart file)
	//

	if (argc != 2 && argc != 4 && argc != 6 && argc != 8 && argc != 10 && argc != 12) {
		cout << "ERROR: Wrong number of arguments to Sampling analysis" << endl;
		return TCL_ERROR;
	}


	// Declaration of input parameters
	int numberOfSimulations = 1000;
	double targetCOV = 0.05;
	double samplingVariance = 1.0;
	int printFlag = 0;
	int analysisTypeTag = 1;


	for (int i = 2; i < argc; i = i + 2) {

		if (strcmp(argv[i], "-type") == 0) {

			if (strcmp(argv[i + 1], "failureProbability") == 0) {

				analysisTypeTag = 1;

				//				if (theStartPoint == 0 ) {
				//					cout << "Need theStartPoint before a SimulationAnalyis can be created" << endl;
				//					return TCL_ERROR;
				//				}
			}

			// Michele and Quan -------------------------
			else if (strcmp(argv[i + 1], "outCrossingFailureProbability") == 0) {

				analysisTypeTag = 4;

				//				if (theStartPoint == 0 ) {
				//					cout << "Need theStartPoint before a SimulationAnalyis can be created" << endl;
				//					return TCL_ERROR;
				//				}
			}



			else if ((strcmp(argv[i + 1], "responseStatistics") == 0) || (strcmp(argv[i + 1], "saveGvalues") == 0)) {

				if (strcmp(argv[i + 1], "responseStatistics") == 0) {
					analysisTypeTag = 2;
				}
				else {
					analysisTypeTag = 3;
				}
				if (samplingVariance != 1.0) {
					cout << "ERROR:: sampling variance must be 1.0 for " << endl
						<< " response statistics sampling." << endl;
					return TCL_ERROR;
				}
				// Make sure that the mean point is the sampling center
				int nrv = theReliabilityDomain->getNumberOfRandomVariables();
				RandomVariable *aRandomVariable;
				if (theStartPoint == 0) {
					theStartPoint = new Vector(nrv);
				}
				for (int i = 1; i <= nrv; i++)
				{
					aRandomVariable = theReliabilityDomain->getRandomVariablePtr(i);
					if (aRandomVariable == 0) {
						cout << "ERROR: when creating theStartPoint - could not find" << endl
							<< " random variable with tag #" << i << "." << endl;
						return TCL_ERROR;
					}
					(*theStartPoint)(i - 1) = aRandomVariable->getMean();
				}
				cout << "NOTE: The startPoint is set to the Mean due to the selected sampling analysis type." << endl;
			}
			else {
				cout << "ERROR: invalid input: type \n";
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[i], "-variance") == 0) {
			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[i + 1], &samplingVariance) != TCL_OK) {
				cout << "ERROR: invalid input: samplingVariance \n";
				return TCL_ERROR;
			}
			if (analysisTypeTag == 2 && samplingVariance != 1.0) {
				cout << "ERROR:: sampling variance must be 1.0 for " << endl
					<< " response statistics sampling." << endl;
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[i], "-maxNum") == 0) {
			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[i + 1], &numberOfSimulations) != TCL_OK) {
				cout << "ERROR: invalid input: numberOfSimulations \n";
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[i], "-targetCOV") == 0) {
			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[i + 1], &targetCOV) != TCL_OK) {
				cout << "ERROR: invalid input: targetCOV \n";
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[i], "-print") == 0) {
			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[i + 1], &printFlag) != TCL_OK) {
				cout << "ERROR: invalid input: printFlag \n";
				return TCL_ERROR;
			}
		}
		else {
			cout << "ERROR: invalid input to sampling analysis. " << endl;
			return TCL_ERROR;
		}
	}

	// Warn about illegal combinations
	if (analysisTypeTag == 2 && printFlag == 2) {
		cout << "ERROR:: The restart option of the sampling analysis cannot be " << endl
			<< " used together with the response statistics option. " << endl;
		return TCL_ERROR;
	}


	theImportanceSamplingAnalysis
		= new ImportanceSamplingAnalysis(interp, theReliabilityDomain,
			theProbabilityTransformation,
			theGFunEvaluator,
			theRandomNumberGenerator,
			numberOfSimulations,
			targetCOV,
			samplingVariance,
			printFlag,
			argv[1],
			theStartPoint,
			analysisTypeTag);

	if (theImportanceSamplingAnalysis == 0) {
		cout << "ERROR: could not create theImportanceSamplingAnalysis \n";
		return TCL_ERROR;
	}

	// Now run analysis
	theImportanceSamplingAnalysis->analyze();
	
	return TCL_OK;

}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runGFunVisualizationAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theGFunVisualizationAnalysis != 0) {
		delete theGFunVisualizationAnalysis;
		theGFunVisualizationAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	if (theGFunEvaluator == 0) {
		cout << "Need theGFunEvaluator before a GFunVisualizationAnalysis can be created" << endl;
		return TCL_ERROR;
	}

	if (theProbabilityTransformation == 0) {
		cout << "Need theProbabilityTransformation before a GFunVisualizationAnalysis can be created" << endl;
		return TCL_ERROR;
	}


	// Initial declarations
	int zeroFindingAlg = 0;

	int rv1 = 0;
	int rv2 = 0;
	int numPts1 = 0;
	int numPts2 = 0;
	double from1 = 0.0;
	double to1 = 0.0;
	double from2 = 0.0;
	double to2 = 0.0;

	int rvDir;
	Matrix theMatrix;
	int numLinePts;
	Vector theDirectionVector;
	Vector axesVector;
	int convFileArgv = 0;

	// Tags to keep track of which options the users chooses
	// (and to check which ones have been given)
	int convResults = 0;
	int space = 0;
	int funSurf = 0;
	int dir = 0;
	int axes = 0;


	// Loop through arguments
	int argvCounter = 2;
	while (argc > argvCounter) {
		if (strcmp(argv[argvCounter], "-convResults") == 0) {
			argvCounter++;

			convFileArgv = argvCounter;
			if ((argc - 1) < argvCounter) {
				cout << "ERROR: No file name found for visualization of convergence results. " << endl;
				return TCL_ERROR;
			}
			argvCounter++;

			convResults = 1;
		}
		else if (strcmp(argv[argvCounter], "-space") == 0) {
			argvCounter++;

			if (strcmp(argv[argvCounter], "X") == 0 || strcmp(argv[argvCounter], "x") == 0) {
				space = 1;
			}
			else if (strcmp(argv[argvCounter], "Y") == 0 || strcmp(argv[argvCounter], "y") == 0) {
				space = 2;
			}
			else {
				cout << "ERROR: Invalid input to visualization analysis. " << endl;
				return TCL_ERROR;
			}
			argvCounter++;
		}
		else if (strcmp(argv[argvCounter], "-funSurf") == 0) {
			argvCounter++;

			if (strcmp(argv[argvCounter], "function") == 0) {
				funSurf = 1;
			}
			else if (strcmp(argv[argvCounter], "surface") == 0) {
				funSurf = 2;
			}
			else {
				cout << "ERROR: Invalid input to visualization analysis. " << endl;
				return TCL_ERROR;
			}
			argvCounter++;
		}
		else if (strcmp(argv[argvCounter], "-dir") == 0) {
			argvCounter++;

			if (strcmp(argv[argvCounter], "rv") == 0) {
				argvCounter++;

				dir = 1;

				// GET INPUT PARAMETER (integer)
				if (Tcl_GetInt(interp, argv[argvCounter], &rvDir) != TCL_OK) {
					cout << "ERROR: invalid input: rvDir  in theGFunVisualizationAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;

			}
			else if (strcmp(argv[argvCounter], "file") == 0) {
				argvCounter++;

				dir = 2;

				// Open file where the vectors are given
				ifstream inputFile(argv[argvCounter], ios::in);
				if (inputFile.fail()) {
					cout << "File " << *argv[argvCounter] << " could not be opened. " << endl;
					return TCL_ERROR;
				}

				// Loop through file to see how many entries there are
				int numRVs = theReliabilityDomain->getNumberOfRandomVariables();
				double dummy;
				int numEntries = 0;
				while (inputFile >> dummy) {
					numEntries++;
				}
				if (numEntries == 0) {
					cout << "ERROR: No entries in the direction file read by visualization analysis!" << endl;
					return TCL_ERROR;
				}

				// Check that the number of points are ok
				if (numEntries != numRVs) {
					cout << "ERROR: Wrong number of entries in the the file " << argv[argvCounter] << endl;
					return TCL_ERROR;
				}

				// Close the file
				inputFile.close();

				// Open it again, now being ready to store the results in a matrix
				ifstream inputFile2(argv[argvCounter], ios::in);
				if (inputFile2.fail()) {
					cout << "File " << *argv[argvCounter] << " could not be opened. " << endl;
					return TCL_ERROR;
				}
				argvCounter++;

				// Store the vector
				Vector dummyDirectionVector(numRVs);
				for (int i = 0; i < numRVs; i++) {
					inputFile2 >> dummyDirectionVector(i);
				}
				inputFile2.close();

				theDirectionVector = dummyDirectionVector;

				//	argvCounter++;   -- wrong  Quan
			}
			else {
				cout << "ERROR: Invalid input to visualization analysis. " << endl;
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[argvCounter], "-coords1") == 0) {

			axes = 1;
			argvCounter++;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[argvCounter], &rv1) != TCL_OK) {
				cout << "ERROR: invalid input: rv1  in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &from1) != TCL_OK) {
				cout << "ERROR: invalid input: from1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &to1) != TCL_OK) {
				cout << "ERROR: invalid input: to1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetInt(interp, argv[argvCounter], &numPts1) != TCL_OK) {
				cout << "ERROR: invalid input: numPts1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			Vector dummy(4);
			dummy(0) = (double)rv1;
			dummy(1) = from1;
			dummy(2) = to1;
			dummy(3) = (double)numPts1;
			axesVector = dummy;

		}
		else if (strcmp(argv[argvCounter], "-coords2") == 0) {

			axes = 2;
			argvCounter++;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[argvCounter], &rv1) != TCL_OK) {
				cout << "ERROR: invalid input: rv1  in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &from1) != TCL_OK) {
				cout << "ERROR: invalid input: from1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &to1) != TCL_OK) {
				cout << "ERROR: invalid input: to1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetInt(interp, argv[argvCounter], &numPts1) != TCL_OK) {
				cout << "ERROR: invalid input: numPts1 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[argvCounter], &rv2) != TCL_OK) {
				cout << "ERROR: invalid input: rv2  in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &from2) != TCL_OK) {
				cout << "ERROR: invalid input: from2 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[argvCounter], &to2) != TCL_OK) {
				cout << "ERROR: invalid input: to2 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			// GET INPUT PARAMETER (double)
			if (Tcl_GetInt(interp, argv[argvCounter], &numPts2) != TCL_OK) {
				cout << "ERROR: invalid input: numPts2 in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;

			Vector dummy(8);
			dummy(0) = (double)rv1;
			dummy(1) = from1;
			dummy(2) = to1;
			dummy(3) = (double)numPts1;
			dummy(4) = (double)rv2;
			dummy(5) = from2;
			dummy(6) = to2;
			dummy(7) = (double)numPts2;
			axesVector = dummy;

		}
		else if (strcmp(argv[argvCounter], "-file") == 0) {

			axes = 3;
			argvCounter++;

			// Open file where the vectors are given
			ifstream inputFile(argv[argvCounter], ios::in);
			if (inputFile.fail()) {
				cout << "File " << *argv[argvCounter] << " could not be opened. " << endl;
				return TCL_ERROR;
			}

			// Loop through file to see how many entries there are
			int numRVs = theReliabilityDomain->getNumberOfRandomVariables();
			int numVectors;
			double dummy;
			int numEntries = 0;
			while (inputFile >> dummy) {
				numEntries++;
			}
			if (numEntries == 0) {
				cout << "ERROR: No entries in the file read by visualization analysis!" << endl;
				return TCL_ERROR;
			}

			// Check that the number of points are ok
			if ((numEntries % numRVs) != 0.0) {
				cout << "ERROR: Wrong number of entries in the the file " << argv[argvCounter] << endl;
				return TCL_ERROR;
			}
			numVectors = (int)(numEntries / numRVs);

			// Close the file
			inputFile.close();

			// Open it again, now being ready to store the results in a matrix
			ifstream inputFile2(argv[argvCounter], ios::in);
			if (inputFile2.fail()) {
				cout << "File " << *argv[argvCounter] << " could not be opened. " << endl;
				return TCL_ERROR;
			}
			argvCounter++;

			// Store the vectors in a matrix
			Matrix dummyMatrix(numRVs, numVectors);
			for (int i = 0; i < numVectors; i++) {
				for (int j = 0; j < numRVs; j++) {
					inputFile2 >> dummyMatrix(j, i);
				}
			}
			inputFile2.close();

			theMatrix = dummyMatrix;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[argvCounter], &numLinePts) != TCL_OK) {
				cout << "ERROR: invalid input: numPts  in theGFunVisualizationAnalysis \n";
				return TCL_ERROR;
			}
			argvCounter++;
		}
		else if (strcmp(argv[argvCounter], "-zeroFindingAlgorithm") == 0) {
			argvCounter++;
			if (strcmp(argv[argvCounter], "safeguardedZeroFinding") == 0) {
				zeroFindingAlg = 1;
				argvCounter++;
			}

		}
		else {
			cout << "ERROR: invalid input to theGFunVisualizationAnalysis." << endl;
			return TCL_ERROR;
		}
	}


	// Check that the input was more or less reasonable
	// convResults [ 0:no,                   1:yes                 ]
	// space       [ 0:error,                1:X,        2:Y       ]
	// funSurf     [ 0:error,                1:function, 2:surface ]
	// dir         [ 0:(needed for surface), 1:rv,       2:file    ] (pass rvDir or theDirectionVector)
	// axes        [ 0:error,   1:coords1,   2:coords2,  3:file    ] (pass axesVector... or theMatrix+numLinePts)

	if (space == 0 || funSurf == 0 || axes == 0) {
		cout << "ERROR: Some input is missing to the visualization analysis." << endl;
		return TCL_ERROR;
	}
	if (dir == 0 && funSurf == 2) {
		cout << "A direction is needed for visualization of the limit-state surface." << endl;
		return TCL_ERROR;
	}

	if (zeroFindingAlg == 0)
		theGFunVisualizationAnalysis = new GFunVisualizationAnalysis(
			theReliabilityDomain,
			theGFunEvaluator,
			theProbabilityTransformation,
			argv[1],
			argv[convFileArgv],
			convResults,
			space,
			funSurf,
			axes,
			dir);
	
	// Pass stuff to the analysis object
	if (dir == 1) {
		theGFunVisualizationAnalysis->setDirection(rvDir);
	}
	else if (dir == 2) {
		theGFunVisualizationAnalysis->setDirection(theDirectionVector);
	}

	if (axes == 1 || axes == 2) {
		theGFunVisualizationAnalysis->setAxes(axesVector);
	}
	else if (axes == 3) {
		theGFunVisualizationAnalysis->setAxes(theMatrix);
		theGFunVisualizationAnalysis->setNumLinePts(numLinePts);
	}

	if (axes == 1 || axes == 2) {

		//		if (theStartPoint == 0 ) {
		//			cout << "Need theStartPoint before this GFunVisualizationAnalysis can be created" << endl;
		//			return TCL_ERROR;
		//		}

		theGFunVisualizationAnalysis->setStartPoint(theStartPoint);
	}

	if (convResults == 1) {

		if (theGradGEvaluator == 0) {
			cout << "Need theGradGEvaluator before this GFunVisualizationAnalysis can be created" << endl;
			return TCL_ERROR;
		}

		if (theMeritFunctionCheck == 0) {
			cout << "Need theMeritFunctionCheck before this GFunVisualizationAnalysis can be created" << endl;
			return TCL_ERROR;
		}

		if (theReliabilityConvergenceCheck == 0) {
			cout << "Need theReliabilityConvergenceCheck before this GFunVisualizationAnalysis can be created" << endl;
			return TCL_ERROR;
		}

		theGFunVisualizationAnalysis->setGradGEvaluator(theGradGEvaluator);
		theGFunVisualizationAnalysis->setMeritFunctionCheck(theMeritFunctionCheck);
		theGFunVisualizationAnalysis->setReliabilityConvergenceCheck(theReliabilityConvergenceCheck);
	}

	if (funSurf == 2) {

		if (theRootFindingAlgorithm == 0) {
			cout << "Need theRootFindingAlgorithm before this GFunVisualizationAnalysis can be created" << endl;
			return TCL_ERROR;
		}

		theGFunVisualizationAnalysis->setRootFindingAlgorithm(theRootFindingAlgorithm);
	}


	// It is chosen to have only one constructor. 
	// Hence, pass some stuff via methods, depending on analysis options. 


	if (theGFunVisualizationAnalysis == 0) {
		cout << "ERROR: could not create theGFunVisualizationAnalysis \n";
		return TCL_ERROR;
	}

	// Now run the analysis
	theGFunVisualizationAnalysis->analyze();
	
	return TCL_OK;
}

//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_runSystemAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
	// In case this is a replacement
	if (theSystemAnalysis != 0) {
		delete theSystemAnalysis;
		theSystemAnalysis = 0;
	}


	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	if (argc != 3) {
		cout << "ERROR: Wrong number of arguments to System Reliability analysis" << endl;
		return TCL_ERROR;
	}


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT
	if (strcmp(argv[2], "allInSeries") == 0) {

		theSystemAnalysis = new SystemAnalysis(theReliabilityDomain, argv[1]);

	}
	else {
		cout << "ERROR: Invalid input to system reliability analysis" << endl;
		return TCL_ERROR;
	}


	if (theSystemAnalysis == 0) {
		cout << "ERROR: Could not create theSystemAnalysis. " << endl;
		return TCL_ERROR;
	}


	// Now run the analysis
	theSystemAnalysis->analyze();
	
	return TCL_OK;
}


//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addFindCurvatures(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
	// In case this is a replacement
	if (theFindCurvatures != 0) {
		delete theFindCurvatures;
		theFindCurvatures = 0;
	}


	// GET INPUT PARAMETER (string) AND CREATE THE OBJECT   // modified by Quan Gu July 2006
	if (strcmp(argv[1], "firstPrincipal") == 0) {

		theFindCurvatures = new FirstPrincipalCurvature();
		if (argc >= 3) {
			if (strcmp(argv[2], "-exe") == 0)
				theFindCurvatures->computeCurvatures(theReliabilityDomain);
		}
	}
	else if (strcmp(argv[1], "bySearchAlgorithm") == 0) {

		// Check that the necessary ingredients are present
		if (theFindDesignPointAlgorithm == 0) {
			cout << "Need theFindDesignPointAlgorithm before a CurvaturesBySearchAlgorithm can be created" << endl;
			return TCL_ERROR;
		}

		int numberOfCurvatures;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[2], &numberOfCurvatures) != TCL_OK) {
			cout << "ERROR: invalid input: numberOfCurvatures \n";
			return TCL_ERROR;
		}

		theFindCurvatures = new CurvaturesBySearchAlgorithm(numberOfCurvatures, theFindDesignPointAlgorithm);
	}
	else {
		cout << "ERROR: unrecognized type of FindCurvatures \n";
		return TCL_ERROR;
	}

	if (theFindCurvatures == 0) {
		cout << "ERROR: could not create theFindCurvatures \n";
		return TCL_ERROR;
	}
	return TCL_OK;
}

////////////////////////////////////
int TclReliabilityModelBuilder_runStructureAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
	    HINSTANCE hNewExe = ShellExecuteA(NULL, "open", argv[1], argv[2], NULL, SW_SHOW);
		printf("Successed Run structure Analysis by OpenSees!\n");
	
	return TCL_OK;
}
///////////////////////////////
///Command:  runMonteCarloResponseAnalysis  -outPutFile  m.out -maxNum 1000 -print 1 -tclFileToRun test.tcl <-seed 1>
int
TclReliabilityModelBuilder_runMonteCarloResponseAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
	// In case this is a replacement
	if (theMonteCarloResponseAnalysis != 0) {
		delete theMonteCarloResponseAnalysis;
		theMonteCarloResponseAnalysis = 0;
	}

	clock_t start;
	start = clock();
	//int seed = 1;
	int seed = (int)start;

	// Do input check
	char theCommand[15] = "inputCheck";
	Tcl_Eval(interp, theCommand);


	// Check for essential tools
	if (theProbabilityTransformation == 0) {
		cout << "Need theProbabilityTransformation before a SimulationAnalyis can be created" <<endl;
		return TCL_ERROR;
	}

	if (theGFunEvaluator == 0) {
		cout << "Need theGFunEvaluator before a SimulationAnalyis can be created" << endl;
		return TCL_ERROR;
	}

	if (theRandomNumberGenerator == 0) {
		cout << "Need theRandomNumberGenerator before a SimulationAnalyis can be created" <<endl;
		return TCL_ERROR;
	}

	int numberOfArguments = argc;
	if (numberOfArguments < 4) {
		cout << "ERROR: invalid number of arguments to designVariable command " <<endl;
		cout << "command: runMonteCarloResponseAnalysis  -outPutFile  m.out -maxNum 1000 -print 1 -tclFileToRun test.tcl" <<endl;
		return TCL_ERROR;
	}

	/* refer
				MonteCarloResponseAnalysis(
							ReliabilityDomain *passedReliabilityDomain,
							ProbabilityTransformation *passedProbabilityTransformation,
							RandomNumberGenerator *passedRandomNumberGenerator,
							int passedNumberOfSimulations,
							int printFlag,
							TCL_Char *outputFileName,
							TCL_Char *tclFileToRunFileName)
	{

	  */
	  // Declaration of input parameters
	int numberOfSimulations = 1000;
	int printFlag = 0;
	char outPutFile[25] = "";
	char * tclFileName = 0;

	int argvCounter = 1;
	while (argc > argvCounter) {
		if ((strcmp(argv[argvCounter], "-maxNum") == 0) || (strcmp(argv[argvCounter], "-maxnum") == 0)) {

			argvCounter++;

			if (Tcl_GetInt(interp, argv[argvCounter], &numberOfSimulations) != TCL_OK) {
				cout << "ERROR: invalid input: numberOfSimulations \n";
				return TCL_ERROR;
			}

			argvCounter++;
		}// if

		else if ((strcmp(argv[argvCounter], "-print") == 0) || (strcmp(argv[argvCounter], "-printFlag") == 0)) {

			argvCounter++;

			if (Tcl_GetInt(interp, argv[argvCounter], &printFlag) != TCL_OK) {
				cout << "ERROR: invalid input: printFlag \n";
				return TCL_ERROR;
			}

			argvCounter++;
		}// if
		else if ((strcmp(argv[argvCounter], "-outPutFile") == 0) || (strcmp(argv[argvCounter], "-outputfile") == 0)) {

			argvCounter++;

			strcpy(outPutFile, argv[argvCounter]);

			argvCounter++;
		}// else if
		/*
		else if ((strcmp(argv[argvCounter], "-tclFileToRun") == 0) || (strcmp(argv[argvCounter], "-TclFileToRun") == 0)) {
			argvCounter++;
			tclFileName = new char[25];
			strcpy(tclFileName, argv[argvCounter]);
			argvCounter++;
		}// else if
		*/
		else if (strcmp(argv[argvCounter], "-seed") == 0) {
			argvCounter++;

			if (Tcl_GetInt(interp, argv[argvCounter], &seed) != TCL_OK) {
				cout << "ERROR: invalid input: seed \n";
				return TCL_ERROR;
			}
			argvCounter++;
		}// else if

		else {
			cout << "warning: unknown command: " << argv[argvCounter] <<endl;
			argvCounter++;

		} //else

	};  // while



	theMonteCarloResponseAnalysis
		= new MonteCarloResponseAnalysis(theGFunEvaluator,
			theReliabilityDomain,
			interp,
			theProbabilityTransformation,
			theRandomNumberGenerator,
			numberOfSimulations,
			printFlag,
			outPutFile,
			//tclFileName,
			seed);


	if (theMonteCarloResponseAnalysis == 0) {
		cout << "ERROR: could not create theMonteCarloResponseAnalysis \n";
		return TCL_ERROR;
	}

	if (tclFileName != 0) delete[] tclFileName;

	// Now run analysis
	theMonteCarloResponseAnalysis->analyze();

	return TCL_OK;

}


//////////////////////////////////////////////////////////////////
int
TclReliabilityModelBuilder_addModulatingFunction(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
{
		ModulatingFunction *theModulatingFunction = 0;

	if (strcmp(argv[2], "gamma") == 0) {

		if (argc != 7) {
			cout << "ERROR: Incorrect number of arguments to gamma modulating function" << endl;
			return TCL_ERROR;
		}

		int thisTag, filterTag;
		double a, b, c;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[1], &thisTag) != TCL_OK) {
			cout << "ERROR: invalid input: tag \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[3], &filterTag) != TCL_OK) {
			cout << "ERROR: invalid input: filterTag \n";
			return TCL_ERROR;
		}
		
		// CHECK THAT THE FILTER EXISTS
		Filter *theFilter = 0;
		theFilter = theReliabilityDomain->getFilter(filterTag);
		if (theFilter == 0) {
			cout << "ERROR: Could not find the filter with tag " << filterTag << endl;
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &a) != TCL_OK) {
			cout << "ERROR: invalid input: a \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[5], &b) != TCL_OK) {
			cout << "ERROR: invalid input: b \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[6], &c) != TCL_OK) {
			cout << "ERROR: invalid input: c \n";
			return TCL_ERROR;
		}
		
		// CREATE THE OBJECT
		theModulatingFunction = new GammaModulatingFunction(thisTag, theFilter, a, b, c);

		if (theModulatingFunction == 0) {
			cout << "ERROR: ran out of memory creating modulating function \n";
			cout << "modulating function: " << thisTag << endl;
			return TCL_ERROR;
		}
		
		// ADD THE OBJECT TO THE DOMAIN
		if (theReliabilityDomain->addModulatingFunction(theModulatingFunction) == false) {
			cout << "ERROR: failed to add modulating function to the domain\n";
			cout << "modulating function: " << thisTag << endl;
			delete theModulatingFunction; // otherwise memory leak
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[2], "constant") == 0) {

		int thisTag, filterTag;
		double amplitude = 0.0;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[1], &thisTag) != TCL_OK) {
			cout << "ERROR: invalid input: tag \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[3], &filterTag) != TCL_OK) {
			cout << "ERROR: invalid input: filterTag \n";
			return TCL_ERROR;
		}

		// CHECK THAT THE FILTER EXISTS
		Filter *theFilter = 0;
		theFilter = theReliabilityDomain->getFilter(filterTag);
		if (theFilter == 0) {
			cout << "ERROR: Could not find the filter with tag " << filterTag << endl;
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &amplitude) != TCL_OK) {
			cout << "ERROR: invalid input: amplitude \n";
			return TCL_ERROR;
		}

		// CREATE THE OBJECT
		theModulatingFunction = new ConstantModulatingFunction(thisTag, theFilter, amplitude);

		if (theModulatingFunction == 0) {
			cout << "ERROR: ran out of memory creating modulating function \n";
			cout << "modulating function: " << thisTag << endl;
			return TCL_ERROR;
		}

		// ADD THE OBJECT TO THE DOMAIN
		if (theReliabilityDomain->addModulatingFunction(theModulatingFunction) == false) {
			cout << "ERROR: failed to add modulating function to the domain\n";
			cout << "modulating function: " << thisTag << endl;
			delete theModulatingFunction; // otherwise memory leak
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[2], "trapezoidal") == 0) {

		int thisTag, filterTag;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[1], &thisTag) != TCL_OK) {
			cout << "ERROR: invalid input: tag \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[3], &filterTag) != TCL_OK) {
			cout << "ERROR: invalid input: filterTag \n";
			return TCL_ERROR;
		}

		// CHECK THAT THE FILTER EXISTS
		Filter *theFilter = 0;
		theFilter = theReliabilityDomain->getFilter(filterTag);
		if (theFilter == 0) {
			cout << "ERROR: Could not find the filter with tag " << filterTag << endl;
			return TCL_ERROR;
		}

		double t1, t2, t3, t4, amplitude;

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &t1) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t1 \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[5], &t2) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t2 \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[6], &t3) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t3 \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[7], &t4) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t4 \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[8], &amplitude) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: amplitude \n";
			return TCL_ERROR;
		}
		
		// CREATE THE OBJECT
		theModulatingFunction = new TrapezoidalModulatingFunction(thisTag, theFilter, t1, t2, t3, t4, amplitude);

		if (theModulatingFunction == 0) {
			cout << "ERROR: ran out of memory creating modulating function \n";
			cout << "modulating function: " << thisTag << endl;
			return TCL_ERROR;
		}

		// ADD THE OBJECT TO THE DOMAIN
		if (theReliabilityDomain->addModulatingFunction(theModulatingFunction) == false) {
			cout << "ERROR: failed to add modulating function to the domain\n";
			cout << "modulating function: " << thisTag << endl;
			delete theModulatingFunction; // otherwise memory leak
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[2], "Koo") == 0) {

		int thisTag, filterTag;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[1], &thisTag) != TCL_OK) {
			cout << "ERROR: invalid input: tag \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[3], &filterTag) != TCL_OK) {
			cout << "ERROR: invalid input: filterTag \n";
			return TCL_ERROR;
		}

		// CHECK THAT THE FILTER EXISTS
		Filter *theFilter = 0;
		theFilter = theReliabilityDomain->getFilter(filterTag);
		if (theFilter == 0) {
			cout << "ERROR: Could not find the filter with tag " << filterTag << endl;
			return TCL_ERROR;
		}

		double t1, t2;

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &t1) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t1 \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[5], &t2) != TCL_OK) {
			cout << "ERROR: invalid input to modulating function: t2 \n";
			return TCL_ERROR;
		}

		
		// CREATE THE OBJECT
		theModulatingFunction = new KooModulatingFunction(thisTag, theFilter, t1, t2);

		if (theModulatingFunction == 0) {
			cout << "ERROR: ran out of memory creating modulating function \n";
			cout << "modulating function: " << thisTag << endl;
			return TCL_ERROR;
		}

		// ADD THE OBJECT TO THE DOMAIN
		if (theReliabilityDomain->addModulatingFunction(theModulatingFunction) == false) {
			cout << "ERROR: failed to add modulating function to the domain\n";
			cout << "modulating function: " << thisTag << endl;
			delete theModulatingFunction; // otherwise memory leak
			return TCL_ERROR;
		}
	}
	else {
		cout << "ERROR:: Unknown type of modulating function. " << endl;
		return TCL_ERROR;
	}
	
		return TCL_OK;
	}
//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	int
		TclReliabilityModelBuilder_addFilter(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		Filter *theFilter = 0;

	
		int tag;
		double period_Tn, damping;

		// GET INPUT PARAMETER (integer)
		if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
			cout << "ERROR: invalid input: tag \n";
			return TCL_ERROR;
		}
		// Quan and Michele
		
		if ((strcmp(argv[2], "delta") == 0) || (strcmp(argv[2], "Delta") == 0)) {

			theFilter = new DeltaFilter(tag);

			if (theFilter == 0) {
				cout << "ERROR: ran out of memory creating filter \n";
				cout << "filter: " << tag << endl;
				return TCL_ERROR;
			}
		
			
			// ADD THE OBJECT TO THE DOMAIN
			if (theReliabilityDomain->addFilter(theFilter) == false) {
				cout << "ERROR: failed to add filter to the domain\n";
				cout << "filter: " << tag << endl;
				delete theFilter; // otherwise memory leak
				return TCL_ERROR;
			}


			return TCL_OK;

		}    // if "constant"


		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[3], &period_Tn) != TCL_OK) {
			cout << "ERROR: invalid input: freq_wn \n";
			return TCL_ERROR;
		}

		// GET INPUT PARAMETER (double)
		if (Tcl_GetDouble(interp, argv[4], &damping) != TCL_OK) {
			cout << "ERROR: invalid input: damping \n";
			return TCL_ERROR;
		}

		

		if ((strcmp(argv[2], "standard") == 0) || (strcmp(argv[2], "standardDisplacement") == 0)) {

			theFilter = new StandardLinearOscillatorDisplacementFilter(tag, period_Tn, damping);
		}
		
		else if (strcmp(argv[2], "Koo") == 0) {

			theFilter = new KooFilter(tag, period_Tn, damping);
		}
		
		else if (strcmp(argv[2], "standardVelocity") == 0) {

			theFilter = new StandardLinearOscillatorVelocityFilter(tag, period_Tn, damping);
		}
		
		else if (strcmp(argv[2], "standardAcceleration") == 0) {

			theFilter = new StandardLinearOscillatorAccelerationFilter(tag, period_Tn, damping);
		}
		else {
			cout << "ERROR:: Unknown type of filter. " << endl;
			return TCL_ERROR;
		}


		if (theFilter == 0) {
			cout << "ERROR: ran out of memory creating filter \n";
			cout << "filter: " << tag << endl;
			return TCL_ERROR;
		}

		// ADD THE OBJECT TO THE DOMAIN
		if (theReliabilityDomain->addFilter(theFilter) == false) {
			cout << "ERROR: failed to add filter to the domain\n";
			cout << "filter: " << tag << endl;
			delete theFilter; // otherwise memory leak
			return TCL_ERROR;
		}
		
		
		return TCL_OK;
	}

	//////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////
	int
		TclReliabilityModelBuilder_addSpectrum(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		Spectrum *theSpectrum = 0;

		if (strcmp(argv[2], "jonswap") == 0) {

			int tag;
			double minFreq, maxFreq, alpha, wp, gamma;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
				cout << "ERROR: invalid input: tag \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[3], &minFreq) != TCL_OK) {
				cout << "ERROR: invalid input: minFreq \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[4], &maxFreq) != TCL_OK) {
				cout << "ERROR: invalid input: maxFreq \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[5], &alpha) != TCL_OK) {
				cout << "ERROR: invalid input: alpha \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[6], &wp) != TCL_OK) {
				cout << "ERROR: invalid input: wp \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[7], &gamma) != TCL_OK) {
				cout << "ERROR: invalid input: gamma \n";
				return TCL_ERROR;
			}

			// CREATE THE OBJECT 
			theSpectrum = new JonswapSpectrum(tag, minFreq, maxFreq, alpha, wp, gamma);

			if (theSpectrum == 0) {
				cout << "ERROR: ran out of memory creating spectrum \n";
				cout << "spectrum: " << tag << endl;
				return TCL_ERROR;
			}
			
			// ADD THE OBJECT TO THE DOMAIN
			if (theReliabilityDomain->addSpectrum(theSpectrum) == false) {
				cout << "ERROR: failed to add spectrum to the domain\n";
				cout << "spectrum: " << tag << endl;
				delete theSpectrum; // otherwise memory leak
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[2], "narrowband") == 0) {

			int tag;
			double minFreq, maxFreq, amplitude;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
				cout << "ERROR: invalid input: tag \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[3], &minFreq) != TCL_OK) {
				cout << "ERROR: invalid input: minFreq \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[4], &maxFreq) != TCL_OK) {
				cout << "ERROR: invalid input: maxFreq \n";
				return TCL_ERROR;
			}

			// GET INPUT PARAMETER (double)
			if (Tcl_GetDouble(interp, argv[5], &amplitude) != TCL_OK) {
				cout << "ERROR: invalid input: amplitude \n";
				return TCL_ERROR;
			}


			// CREATE THE OBJECT 
			theSpectrum = new NarrowBandSpectrum(tag, minFreq, maxFreq, amplitude);

			if (theSpectrum == 0) {
				cout << "ERROR: ran out of memory creating spectrum \n";
				cout << "spectrum: " << tag << endl;
				return TCL_ERROR;
			}

			// ADD THE OBJECT TO THE DOMAIN
			if (theReliabilityDomain->addSpectrum(theSpectrum) == false) {
				cout << "ERROR: failed to add spectrum to the domain\n";
				cout << "spectrum: " << tag << endl;
				delete theSpectrum; // otherwise memory leak
				return TCL_ERROR;
			}
		}
		else if (strcmp(argv[2], "points") == 0) {

			int tag;
			double frequency, amplitude;

			// GET INPUT PARAMETER (integer)
			if (Tcl_GetInt(interp, argv[1], &tag) != TCL_OK) {
				cout << "ERROR: invalid input: tag \n";
				return TCL_ERROR;
			}

			if (fmod((argc - 3), 2.0)) {
				cout << "ERROR: Inconsistent number of points given to spectrum " << tag << endl;
				return TCL_ERROR;
			}


			int numPoints = (int)((argc - 3) / 2.0);

			Vector frequencies(numPoints);
			Vector amplitudes(numPoints);
			for (int iii = 1; iii <= numPoints; iii++) {

				// GET INPUT PARAMETER (double)
				if (Tcl_GetDouble(interp, argv[(iii - 1) * 2 + 3], &frequency) != TCL_OK) {
					cout << "ERROR: invalid input: frequency \n";
					return TCL_ERROR;
				}

				// GET INPUT PARAMETER (double)
				if (Tcl_GetDouble(interp, argv[(iii - 1) * 2 + 4], &amplitude) != TCL_OK) {
					cout << "ERROR: invalid input: amplitude \n";
					return TCL_ERROR;
				}

				frequencies(iii - 1) = frequency;
				amplitudes(iii - 1) = amplitude;
			}

			

			// CREATE THE OBJECT 
			theSpectrum = new PointsSpectrum(tag, frequencies, amplitudes);

			if (theSpectrum == 0) {
				cout << "ERROR: ran out of memory creating spectrum \n";
				cout << "spectrum: " << tag << endl;
				return TCL_ERROR;
			}

			// ADD THE OBJECT TO THE DOMAIN
			if (theReliabilityDomain->addSpectrum(theSpectrum) == false) {
				cout << "ERROR: failed to add spectrum to the domain\n";
				cout << "spectrum: " << tag << endl;
				delete theSpectrum; // otherwise memory leak
				return TCL_ERROR;
			}
		}
		else {
			cout << "ERROR:: Unknown type of spectrum. " << endl;
			return TCL_ERROR;
		}
		
		return TCL_OK;
	}
//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	int
		TclReliabilityModelBuilder_runParametricReliabilityAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		
		// In case this is a replacement
		if (theParametricReliabilityAnalysis != 0) {
			delete theParametricReliabilityAnalysis;
			theParametricReliabilityAnalysis = 0;
		}


		// Do input check
		char theCommand[15] = "inputCheck";
		Tcl_Eval(interp, theCommand);


		// Check number of arguments
		if (argc != 9) {
			cout << "ERROR: Wrong number of input parameter to Fragility analysis" << endl;
			return TCL_ERROR;
		}


		// Check for essential ingredients
		if (theFindDesignPointAlgorithm == 0) {
			cout << "Need theFindDesignPointAlgorithm before a ParametricReliabilityAnalysis can be created" << endl;
			return TCL_ERROR;
		}
		if (theGradGEvaluator == 0) {
			cout << "Need theGradGEvaluator before a ParametricReliabilityAnalysis can be created" << endl;
			return TCL_ERROR;
		}


		// Read input
		bool parGiven = false;
		bool rangeGiven = false;
		bool numIntGiven = false;
		int parameterNumber;
		double first;
		double last;
		int numIntervals;
		int counter = 3;
		for (int i = 1; i <= 3; i++) {

			if (strcmp(argv[counter - 1], "-par") == 0) {
				// GET INPUT PARAMETER (int)
				if (Tcl_GetInt(interp, argv[counter], &parameterNumber) != TCL_OK) {
					cout << "ERROR: invalid input: parameterNumber \n";
					return TCL_ERROR;
				}
				counter++; counter++;
				parGiven = true;
			}
			else if (strcmp(argv[counter - 1], "-range") == 0) {
				// GET INPUT PARAMETER (double)
				if (Tcl_GetDouble(interp, argv[counter], &first) != TCL_OK) {
					cout << "ERROR: invalid input: first bound to range \n";
					return TCL_ERROR;
				}
				counter++;
				// GET INPUT PARAMETER (double)
				if (Tcl_GetDouble(interp, argv[counter], &last) != TCL_OK) {
					cout << "ERROR: invalid input: last bound to range \n";
					return TCL_ERROR;
				}
				counter++; counter++;
				rangeGiven = true;
			}
			else if (strcmp(argv[counter - 1], "-numInt") == 0) {
				// GET INPUT PARAMETER (int)
				if (Tcl_GetInt(interp, argv[counter], &numIntervals) != TCL_OK) {
					cout << "ERROR: invalid input: number of intervals \n";
					return TCL_ERROR;
				}
				counter++; counter++;
				numIntGiven = true;
			}
			else {
				cout << "ERROR: invalid input to Fragility analysis " << endl;
				return TCL_ERROR;
			}

		}

		if (parGiven && rangeGiven && numIntGiven) {
			theParametricReliabilityAnalysis = new ParametricReliabilityAnalysis(theReliabilityDomain,
				theFindDesignPointAlgorithm,
				theGradGEvaluator,
				parameterNumber,
				first,
				last,
				numIntervals,
				argv[1],
				interp);
		}
		else {
			cout << "ERROR:: some input to theParametricReliabilityAnalysis was not provided" << endl;
			return TCL_ERROR;
		}

		if (theParametricReliabilityAnalysis == 0) {
			cout << "ERROR: could not create theParametricReliabilityAnalysis \n";
			return TCL_ERROR;
		}

		// Now run the analysis
		theParametricReliabilityAnalysis->analyze();
		
		return TCL_OK;
	}

	//////////////////////////////////////////////////////////////////

	// Quan and Michele Feb 2006

// command "runOutCrossingAnalysis  filename?  -results stepsToStart?  stepsToEnd?  samplefreq? impulseFreq?   -littleDt dt? -analysisType 
// option for analysisType 1:   -twoSearches   <-integralTolerance  tol? -useFirstDesignPoint>
//            analysisType 2:    -Koo
//                                       
	int
		TclReliabilityModelBuilder_runOutCrossingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		
		// In case this is a replacement
		if (theOutCrossingAnalysis != 0) {
			delete theOutCrossingAnalysis;
			theOutCrossingAnalysis = 0;
		}


		// Do input check
		char theCommand[15] = "inputCheck";
		Tcl_Eval(interp, theCommand);


		if (theFindDesignPointAlgorithm == 0) {
			cout << "Need theFindDesignPointAlgorithm before an OutCrossingAnalysis can be created" << endl;
			return TCL_ERROR;
		}
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before an OutCrossingAnalysis can be created" << endl;
			return TCL_ERROR;
		}
		if (theGradGEvaluator == 0) {
			cout << "Need theGradGEvaluator before an OutCrossingAnalysis can be created" << endl;
			return TCL_ERROR;
		}

		int stepsToStart = 0;
		int stepsToEnd = 0;
		int sampleFreq = 1;
		double littleDt = 0.01;
		int analysisType = 1;

		int impulseFreq;

		double integralTolerance = 1.e-10;
		bool useFirstDesignPt = false;



		// Loop through arguments
		int argvCounter = 2;
		while (argc > argvCounter) {
			if (strcmp(argv[argvCounter], "-results") == 0) {
				argvCounter++;

				// GET INPUT PARAMETER (integer)
				if (Tcl_GetInt(interp, argv[argvCounter], &stepsToStart) != TCL_OK) {
					cout << "ERROR: invalid input stepsToStart to theOutCrossingAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;

				// GET INPUT PARAMETER (integer)
				if (Tcl_GetInt(interp, argv[argvCounter], &stepsToEnd) != TCL_OK) {
					cout << "ERROR: invalid input stepsToEnd to theOutCrossingAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;

				// GET INPUT PARAMETER (integer)
				if (Tcl_GetInt(interp, argv[argvCounter], &sampleFreq) != TCL_OK) {
					cout << "ERROR: invalid input sampleFreq to theOutCrossingAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &impulseFreq) != TCL_OK) {
					cout << "ERROR: invalid input impulseFreq to theOutCrossingAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-littleDt") == 0) {
				argvCounter++;

				// GET INPUT PARAMETER (double)
				if (Tcl_GetDouble(interp, argv[argvCounter], &littleDt) != TCL_OK) {
					cout << "ERROR: invalid input littleDt to theOutCrossingAnalysis \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}
			else if (strcmp(argv[argvCounter], "-Koo") == 0) {
				argvCounter++;
				analysisType = 2;
			}
			else if (strcmp(argv[argvCounter], "-twoSearches") == 0) {
				argvCounter++;
				analysisType = 1;
				if (argc > argvCounter) {

					if (strcmp(argv[argvCounter], "-useFirstDesignPoint") == 0) {
						argvCounter++;
						useFirstDesignPt = true;

					}
					if (argc > argvCounter) {
						if (strcmp(argv[argvCounter], "-integralTolerance") == 0) {
							argvCounter++;
							if (Tcl_GetDouble(interp, argv[argvCounter], &integralTolerance) != TCL_OK) {
								cout << "ERROR: invalid input littleDt to theOutCrossingAnalysis \n";
								return TCL_ERROR;
							}
							argvCounter++;
						}
					}

				}

			}
			else {
				cout << "ERROR: Invalid input to theOutCrossingAnalysis." << endl;
				return TCL_ERROR;
				argvCounter++;
			}
		}

		theOutCrossingAnalysis
			= new OutCrossingAnalysis(
				theReliabilityDomain,
				theGFunEvaluator,
				theGradGEvaluator,
				theFindDesignPointAlgorithm,
				analysisType,
				stepsToStart,
				stepsToEnd,
				sampleFreq,
				impulseFreq,
				littleDt,
				argv[1],
				integralTolerance,
				useFirstDesignPt);

		if (theOutCrossingAnalysis == 0) {
			cout << "ERROR: could not create theOutCrossingAnalysis \n";
			return TCL_ERROR;
		}

		// Now run analysis
		theOutCrossingAnalysis->analyze();
		
		return TCL_OK;

	}
	//////////////////////////////////////////////////////////////////
	int
		TclReliabilityModelBuilder_runOrthogonalPlaneSamplingAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		
		// In case this is a replacement
		if (theSamplingAnalysis != 0) {
			delete theSamplingAnalysis;
			theSamplingAnalysis = 0;
		}


		// Do input check
		char theCommand[15] = "inputCheck";
		Tcl_Eval(interp, theCommand);


		// Check for essential tools
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}
		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}
		if (theRandomNumberGenerator == 0) {
			cout << "Need theRandomNumberGenerator before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}


		// The following switches are available (default values are provided)
		// (The sampling is performed around theStartPoint, except 
		// for response statistics sampling; then the mean is used together
		// with unit sampling variance.)
		//
		//     -type  failureProbability (1)......... this is the default
		//     -type  outcrossing (2)
		//     -type  saveGvalues (3) .... not yet
		//
		//     -maxNum 1000  ........................ this is the default
		//
		//     -targetCOV 0.05  ..................... this is the default
		//
		//     -print 0   (print nothing) 
		//     -print 1   failure prob. and cov  .... this is the default
		//     -print 2   ............................ restart file  // 2007 Feb. Quan
		//     -print 5    recorder the surface ......this is for visualization
		//     -funcTol  1.e-5 ....................... this is the default
		//     -varTol   1.e-3........................ this is the default
		//     -maxIter   20  .........................this is the default

		if (argc < 3) {
			cout << "command: runOrthogonalPlaneSamplingAnalysis  -fileName filename?  -maxNum number?   -type  analysisType? -targetCOV cov? -print printFlag? ";
			cout << " -funcTol tol1? -varTol tol2? -maxIter iter?" << endl;
			return TCL_ERROR;
		}

		// Declaration of input parameters
		int numberOfSimulations = 1000;
		double targetCOV = 0.05;
		int printFlag = 1;
		double funcTol = 1.e-5;
		double varTol = 1.e-3;
		int maxIter = 20;
		int analysisTypeTag = 1;
		char name[50];
		Vector * theDesignPoint;
		double littleDt = 1.0e-3;

		if (theStartPoint == 0) {
			cout << "orthogonalPlaneSamplingAnalysis can not run. Need StartPoint !" << endl;
			return TCL_ERROR;
		}
		else theDesignPoint = theStartPoint;

		int argvCounter = 1;
		while (argc > argvCounter) {
			if ((strcmp(argv[argvCounter], "-fileName") == 0) || (strcmp(argv[argvCounter], "-filename") == 0)) {
				argvCounter++;
				strcpy(name, argv[argvCounter]);
				argvCounter++;
			}// if

			else if ((strcmp(argv[argvCounter], "-maxNum") == 0) || (strcmp(argv[argvCounter], "-maxnum") == 0)) {
				argvCounter++;
				if (Tcl_GetInt(interp, argv[argvCounter], &numberOfSimulations) != TCL_OK) {
					cout << "ERROR: invalid input: numberOfSimulations \n";
					return TCL_ERROR;
				}
				argvCounter++;
			}// else if

			else if (strcmp(argv[argvCounter], "-type") == 0) {
				argvCounter++;
				if (strcmp(argv[argvCounter], "failureProbability") == 0) {
					analysisTypeTag = 1;
				}
				else if (strcmp(argv[argvCounter], "outCrossing") == 0) {
					analysisTypeTag = 2;
				}
				argvCounter++;
			}

			else if (strcmp(argv[argvCounter], "-targetCOV") == 0) {
				argvCounter++;
				if (Tcl_GetDouble(interp, argv[argvCounter++], &targetCOV) != TCL_OK) {
					cout << "ERROR: invalid input: targetCOV \n";
					return TCL_ERROR;
				}
			}
			else if (strcmp(argv[argvCounter], "-print") == 0) {
				argvCounter++;
				if (Tcl_GetInt(interp, argv[argvCounter++], &printFlag) != TCL_OK) {
					cout << "ERROR: invalid input: printFlag \n";
					return TCL_ERROR;
				}
			}

			else if (strcmp(argv[argvCounter], "-funcTol") == 0) {
				argvCounter++;
				if (Tcl_GetDouble(interp, argv[argvCounter++], &funcTol) != TCL_OK) {
					cout << "ERROR: invalid input: funcTol \n";
					return TCL_ERROR;
				}
			}

			else if (strcmp(argv[argvCounter], "-varTol") == 0) {
				argvCounter++;
				if (Tcl_GetDouble(interp, argv[argvCounter++], &varTol) != TCL_OK) {
					cout << "ERROR: invalid input: varTol \n";
					return TCL_ERROR;
				}
			}

			else if (strcmp(argv[argvCounter], "-maxIter") == 0) {
				argvCounter++;
				if (Tcl_GetInt(interp, argv[argvCounter++], &maxIter) != TCL_OK) {
					cout << "ERROR: invalid input: maxIter \n";
					return TCL_ERROR;
				}
			}
			else if (strcmp(argv[argvCounter], "-littleDt") == 0) {
				argvCounter++;
				if (Tcl_GetDouble(interp, argv[argvCounter++], &littleDt) != TCL_OK) {
					cout << "ERROR: invalid input: littleDt \n";
					return TCL_ERROR;
				}
			}

			else {
				cout << "ERROR: invalid input to sampling analysis. " << endl;
				return TCL_ERROR;
			}
		}


		


		// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx
		theSamplingAnalysis
			= new OrthogonalPlaneSamplingAnalysis(interp,
				theReliabilityDomain,
				theProbabilityTransformation,
				theGFunEvaluator,
				theRandomNumberGenerator,
				numberOfSimulations,
				maxIter,
				targetCOV,
				printFlag,
				name,
				theDesignPoint,
				analysisTypeTag,
				1,
				funcTol,
				varTol,
				maxIter,
				littleDt);

		if (theSamplingAnalysis == 0) {
			cout << "ERROR: could not create theOrthogonalSamplingAnalysis \n";
			return TCL_ERROR;
		}

		// Now run analysis
		theSamplingAnalysis->analyze();

		
		return TCL_OK;

	}


	//////////////////////////////////////////////////////////////////

	int
		TclReliabilityModelBuilder_runSubsetSimulationAnalysis(ClientData clientData, Tcl_Interp *interp, int argc, TCL_Char **argv)
	{
		// In case this is a replacement
		if (theSubsetSimulationAnalysis != 0) {
			delete theSubsetSimulationAnalysis;
			theSubsetSimulationAnalysis = 0;
		}

		clock_t start;
		start = clock();
		int seed = (int)start;
		//int seed = 1;

		// Do input check
		char theCommand[15] = "inputCheck";
		Tcl_Eval(interp, theCommand);

		// Check for essential tools
		if (theProbabilityTransformation == 0) {
			cout << "Need theProbabilityTransformation before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}

		if (theGFunEvaluator == 0) {
			cout << "Need theGFunEvaluator before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}

		if (theRandomNumberGenerator == 0) {
			cout << "Need theRandomNumberGenerator before a SimulationAnalyis can be created" << endl;
			return TCL_ERROR;
		}

		int numberOfArguments = argc;
		if (numberOfArguments < 4) {
			cout << "ERROR: invalid number of arguments to designVariable command " << endl;
			cout << "command: runMonteCarloResponseAnalysis  -outPutFile  m.out -SamplingType HM 1 -maxNum 1000 -print 1" << endl;
			return TCL_ERROR;
		}

		// Declaration of input parameters
		int printFlag = 0;
		char outPutFile[25] = "";
		char * tclFileName = 0;
		char SamplingType[25] = "HMC"; //defaute using Hamiltonian Monte Carlo sampling method
		int SamplingFlag = 1;
		int NumSeedSamples = 1000; //The number of seed samples in each intermediate state of subset simulation

		int argvCounter = 1;
		while (argc > argvCounter) {
			if (strcmp(argv[argvCounter], "-NumSeedSamples") == 0) {

				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &NumSeedSamples) != TCL_OK) {
					cout << "ERROR: invalid input: number of seed samples \n";
					return TCL_ERROR;
				}

				argvCounter++;
			}// if

			else if ((strcmp(argv[argvCounter], "-print") == 0) || (strcmp(argv[argvCounter], "-printFlag") == 0)) {

				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &printFlag) != TCL_OK) {
					cout << "ERROR: invalid input: printFlag \n";
					return TCL_ERROR;
				}

				argvCounter++;
			}// else if

			else if ((strcmp(argv[argvCounter], "-outPutFile") == 0) || (strcmp(argv[argvCounter], "-outputfile") == 0)) {

				argvCounter++;

				strcpy(outPutFile, argv[argvCounter]);

				argvCounter++;
			}// else if

			else if (strcmp(argv[argvCounter], "-seed") == 0) {

				argvCounter++;

				if (Tcl_GetInt(interp, argv[argvCounter], &seed) != TCL_OK) {
					cout << "ERROR: invalid input: seed \n";
					return TCL_ERROR;
				}

				argvCounter++;
			}// else if

			else if (strcmp(argv[argvCounter], "-SamplingType") == 0) {

				argvCounter++;

				strcpy(SamplingType, argv[argvCounter]);

				if (strcmp(SamplingType, "HMC") == 0) {

					argvCounter++;

				}
				else {

					argvCounter++;

					if (Tcl_GetInt(interp, argv[argvCounter], &SamplingFlag) != TCL_OK) {
						cout << "ERROR: invalid input: SamplingFlag \n";
						return TCL_ERROR;
					}

					argvCounter++;
				}
			}//else if

			else {
				cout << "warning: unknown command: " << argv[argvCounter] << endl;
				argvCounter++;
			} //else

		};  // while

		NumSeedSamples = (int)ceil(NumSeedSamples*0.01) * 100;


		theSubsetSimulationAnalysis
			= new SubsetSimulationAnalysis(theGFunEvaluator,
				theReliabilityDomain,
				interp,
				theProbabilityTransformation,
				theRandomNumberGenerator,
				printFlag,
				outPutFile,
				seed,
				SamplingType,
				SamplingFlag,
				NumSeedSamples);


		if (theSubsetSimulationAnalysis == 0) {
			cout << "ERROR: could not create theSubsetSimulationAnalysis \n";
			return TCL_ERROR;
		}

		if (tclFileName != 0) delete[] tclFileName;

		// Now run analysis
		theSubsetSimulationAnalysis->analyze();

		return TCL_OK;
	}