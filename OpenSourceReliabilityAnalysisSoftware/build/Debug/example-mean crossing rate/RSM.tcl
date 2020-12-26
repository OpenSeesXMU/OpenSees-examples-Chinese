

# need designPointX.out, which is 10000 array, include all perturbations.


# model basic -ndm 2 -ndf 2 

# new model: M=28.8e3 kg ; K = 40.56 kN/mm;
# T0=0.1721;
# x = 2%;c = 800 pi kg/s; 
# phi0 = .25 m2/s3 = s2*Dt/(2pi); s = (2pi)^.5 = 2.507 m/s2~.25g
# fy = 10 kN 

# RELIABILITY MODEL BUILDER 
reliability
# set M 28.8e3
# # NODES 
# node 1 0.0 0.0 
# node 2 1.0 0.0 -mass $M $M

# # fy   E   b : strong nonlinearity
# # 200 400 0.05
# set K 40560.0e3

# #uniaxialMaterial Elastic   1     $K  

# #                                    fy     E      b
# #uniaxialMaterial SteelMP   1      734.0e3  $K   0.05 

# #                        $matTag      $E   $sigmaY  $H_iso   $H_kin <$eta>     b=Hkin/(E+Hkin)=0.05 
# uniaxialMaterial Hardening  1         $K   734.0e3   0.0      2.1347e+006

# set omega [expr pow($K/$M,0.5)]
# # element truss $eleTag $iNode $jNode $A $matTag
  # element truss   1        1       2  1.0    1   
 
 
# # BOUNDARY CONDITIONS 
# fix 1 1 1 
# fix 2 0 1 



# filter 1 delta

# # MODULATING FUNCTIONS   type     filterTag   arguments
# modulatingFunction  1   constant     1           1

# # LOAD PATTERN WITH DISCRETIZED RANDOM PROCESS TIME SERIES
# #                                                                         mean                    maxStdev           1stModulatingFunction   2nd...
# pattern UniformExcitation  1   1    -accel {  DiscretizedRandomProcess     0.0                    1.0      1     }
# # RECORDER 
# recorder Node -file nodeDisp_1.out  -time -node 2 -dof 1  disp
# #recorder Node -file nodeForce.out  -time -node 2 -dof 1  disp

# #recorder Node -file nodeAccel.out  -time -node 2 -dof 1  accel
# recorder Element -file axial.out  -time -element 1 axialForce
# recorder Element -file axial.out  -time -element 1 axialDeformation

# # STRUCTURAL ANALYSIS MODEL
# system BandSPD
# constraints Plain
# test NormDispIncr 1.0e-16  50 
# algorithm Newton
# numberer RCM
# #integrator Newmark $gamma $beta <$alphaM $betaK $betaKinit $betaKcomm>
# #integrator NewmarkWithSensitivity   0.5  0.25  [expr 2*0.02*$omega]  0.0   0.0   0.0  
# integrator Newmark   0.5  0.25  [expr 2*0.02*$omega]  0.0   0.0   0.0  
# #2% damping ratio
# #sensitivityIntegrator -definedAbove
# #sensitivityAlgorithm -computeAtEachStep  
# analysis Transient






# TIME-STEPPING PARAMETERS
set totalTime    3.0
set numPulses    10
set numTimeSteps [expr $numPulses*10]
set pi 3.14159265358979
#set pi 3.1415926535898
set phi0 0.25
# UNCERTAINTY CHARACTERIZATION
set deltat [expr $totalTime/$numPulses]

#Dt=.02   8.8622692545275801
#Dt=.01   12.533141373155 
#Dt=.005  17.72453850905516 
for { set i 1 } { $i <= [expr $numPulses] } { incr i } {
	randomVariable $i normal  0.0  [expr pow(2*$pi*$phi0/$deltat,0.5)] 0.0
	#randomVariablePositioner $i -rvNum $i -loadPattern 1 -randomProcessDiscretizer [expr ($i)*$deltat]
}


# PERFORMANCE FUNCTION
performanceFunction 1 "0.0048 - {u_2_1}"

# RELIABILITY ANALYSIS MODEL
probabilityTransformation    Nataf            -print 0
reliabilityConvergenceCheck  Standard         -e1 1.0e-6    -e2 1.0e-6  -print 1
#gFunEvaluator                OpenSees         -analyze $numTimeSteps [expr $totalTime/$numTimeSteps]
gFunEvaluator                OpenSees  Timevariant           -analyze $numTimeSteps [expr $totalTime/$numTimeSteps] -file reliability_3_aux.tcl
#gradGEvaluator               OpenSees         
gradGEvaluator               FiniteDifference   -pert 1000      
searchDirection              iHLRF
meritFunctionCheck           AdkZhang         -multi 2.0    -add 10.0   -factor 0.5  
stepSizeRule                 Armijo           -maxNum 50    -base 0.5   -initial 1.0 2  -print 0
startPoint                   Given
#startPoint 
#startPoint                   -file dp_old.txt -shiftStartPt
#findDesignPoint              SNOPT       -maxNumIter 100 -printDesignPointX designPointX.out
findDesignPoint              StepSearch       -maxNumIter 100 -printDesignPointX designPointX.out
#randomNumberGenerator        MT
randomNumberGenerator        CStdLib
# RUN THE ANALYSIS
#                                                              stepsToStart  stepsToEnd     freq
runOutCrossingAnalysis       OutCross.out     -results        $numTimeSteps  $numTimeSteps  $numTimeSteps  [expr $numTimeSteps/$numPulses]   -littleDt 1.0e-6 -Koo;#-twoSearches;
#runOutCrossingAnalysis       OutCross.out     -results        $numTimeSteps  $numTimeSteps  $numTimeSteps  [expr $numTimeSteps/$numPulses]   -littleDt 1.0e-6 -twoSearches;
#findCurvatures  firstPrincipal -exe


#computeHessian -FDM -file Hessian.out -designPoint designPointX.out -perturbation  1.0e-4
#runMultiDimVisualPrinPlane -funSurf function -designPt designPointX.out  -ndir 3 -output vis.out -gridInfo {0  -1  1  21    1   -1  1  21    2   -1  1  21    3   -1  1  21   } -saveHessian hessian.out  -timeVariant -littleDt 0.005

#set startT [clock seconds]

#runDP_RSM_SimTimeVariantAnalysis -designPt designPointX.out  -output results.out  -ndir 4 -experimentalPointRule Uniform -gridInfo {  0  -4  4  8  1   -4  4  8  2   -4  4   8  3   -4  4   8   4   -4  4   8}   -saveHessian hessian.out -surfaceDesign BivariateDecomposition -simulation ImportanceSampling -tarCOV 0.05 -numSimulation 5000000 -littleDt 0.001 -ImpulseInterval 0.01

#set endT  [clock seconds] ; set duration [expr $endT - $startT] ; puts "Time:  $duration"
