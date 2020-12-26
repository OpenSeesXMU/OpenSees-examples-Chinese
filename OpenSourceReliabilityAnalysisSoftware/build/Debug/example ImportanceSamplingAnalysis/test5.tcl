# CREATE THE RELIABILITY MODEL BUILDER. NO FINITE ELEMENT MODEL BUILDER IS NECESSARY IN THIS EXAMPLE
reliability

# CREATE RANDOM VARIABLES
randomVariable 1 lognormal  500.0   100.0  500.0 
randomVariable 2 lognormal  2000.0  400.0 2000.0 
randomVariable 3 uniform    5.0       0.5    5.0 

# SPECIFY CORRELATION
correlate   1 2  0.3
correlate   1 3  0.2
correlate   2 3  0.2

# LIMIT-STATE FUNCTION
set a "{x_2}/(1000.0*{x_3})"
set b "{x_1}/(200.0*{x_3})"
performanceFunction 1 "1.0 - $a - $b*$b"

# CREATE NECESSARY RELIABILITY ANALYSIS TOOLS
randomNumberGenerator        CStdLib
probabilityTransformation    Nataf            -print 0
reliabilityConvergenceCheck  Standard         -e1 1.0e-3    -e2 1.0e-3  -print 1
gFunEvaluator                Basic
gradGEvaluator               FiniteDifference -pert 1000
searchDirection              iHLRF
meritFunctionCheck           AdkZhang         -multi 2.0    -add 10.0   -factor 0.0  
stepSizeRule                 Armijo           -maxNum 50    -base 0.5   -initial 1.0 2  -print 0
startPoint                   Mean
findDesignPoint              StepSearch       -maxNumIter 100  -printDesignPointX designPoint1.txt

# RUN FORM+IMPORTANCE SAMPLING ANALYSIS
runFORMAnalysis              FORMoutput1.out
startPoint                   -file designPoint1.txt
runImportanceSamplingAnalysis          SIMULATIONoutput1.out -type failureProbability -variance 1.0 -maxNum 10000 -targetCOV 0.01 -print 0
#runSamplingAnalysis          SIMULATIONoutput1.out -type failureProbability -variance 1.0 -maxNum 10000 -targetCOV 0.01 -print 0

