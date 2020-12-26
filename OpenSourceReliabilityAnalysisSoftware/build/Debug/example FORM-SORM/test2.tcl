# CREATE THE RELIABILITY MODEL BUILDER. NO FINITE ELEMENT MODEL BUILDER IS NECESSARY IN THIS EXAMPLE
#reliability

# CREATE RANDOM VARIABLES
randomVariable 1 lognormal  500.0   100.0  500.0 
randomVariable 2 lognormal  2000.0  400.0 2000.0 
randomVariable 3 uniform    5.0       0.5    5.0 

# SPECIFY CORRELATION
correlate   1 2  0.3
correlate   1 3  0.2
correlate   2 3  0.2

# DEFINE A LIMIT-STATE FUNCTION
set a "{x_2}/(1000.0*{x_3})"
set b "{x_1}/(200.0*{x_3})"
performanceFunction 1 "1.0 - $a - $b*$b"

# CREATE NECESSARY RELIABILITY ANALYSIS TOOLS
probabilityTransformation    Nataf            -print 0
reliabilityConvergenceCheck  Standard         -e1 1.0e-3    -e2 1.0e-3  -print 1
gFunEvaluator                Basic
gradGEvaluator               FiniteDifference -pert 1000
rootFinding                  Secant           -maxIter 50   -tol 1.0e-2 -maxStepLength 1.0
stepSizeRule                 Fixed            -stepSize 1.0
searchDirection              GradientProjection  
startPoint                   Mean
findDesignPoint              StepSearch       -maxNumIter 100

# RUN THE FORM ANALYSIS
runFORMAnalysis FORMoutput.out
