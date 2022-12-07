

optimization




# ---------------------------------- optimization part ----------------

designVariable  1 -name DV_E1   -startPt 1.8e8     -lowerBound 1.0E8   -upperBound  1.e20
designVariable  2 -name DV_fy1  -startPt 270000.   -lowerBound 100000.0   -upperBound  1.e20
designVariable  3 -name DV_b1   -startPt 0.016      -lowerBound 0.0   -upperBound  1.e20
designVariable  4 -name DV_E2   -startPt 1.8e8     -lowerBound 1.0E8   -upperBound  1.e20
designVariable  5 -name DV_fy2  -startPt 270000.  -lowerBound 100000.0   -upperBound  1.e20
designVariable  6 -name DV_b2   -startPt 0.016      -lowerBound 0.0   -upperBound  1.e20


#source tclFileToRun.tcl

objectiveFunction  1 -name F -tclFile F.tcl  -lowerBound -1.e20 -upperBound 1.e20 ; #-gradientName gradF 
     
#array set uBound {1 4.0 2 5.0}  
#array set lBound {1 -1e20 2 -1e20}  

#constraintFunction 1  -name G -gradientName gradG  -tclFile G.tcl -upperBound uBound -lowerBound lBound

runSNOPTAnalysis -maxNumIter 50 -printOptPointX OptX.out -printFlag 1  -tclFileToRun tclFileToRun.tcl 

