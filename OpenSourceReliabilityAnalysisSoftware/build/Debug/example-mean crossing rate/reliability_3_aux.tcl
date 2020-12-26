# integrator LoadControl  0.05   3   0.05    0.05
# analyze 16
model basic -ndm 2 -ndf 2 
set zzzzero 0;
set GMScale_sample [open RV.txt "r"];
set GMScale_data [read $GMScale_sample];
close $GMScale_sample;

set fo1 [open RV2.txt w] ;

foreach different_content {"zzzzero" "GMScale_data"} { 
puts $fo1 [set $different_content] 
} 

close $fo1 

 # set E [lindex $GMScale_data 0];
 # set fy [lindex $GMScale_data 1];
 # set P [lindex $GMScale_data 2];
# integrator LoadControl  0.02   3   0.02    0.02
# analyze 10
set M 28.8e3
# NODES 
node 1 0.0 0.0 
node 2 1.0 0.0 -mass $M $M

# fy   E   b : strong nonlinearity
# 200 400 0.05
set K 40560.0e3
set pi 3.14159265358979
#uniaxialMaterial Elastic   1     $K  

#                                    fy     E      b
#uniaxialMaterial SteelMP   1      734.0e3  $K   0.05 

#                        $matTag      $E   $sigmaY  $H_iso   $H_kin <$eta>     b=Hkin/(E+Hkin)=0.05 
uniaxialMaterial Hardening  1         $K   734.0e3   0.0      2.1347e+006

set omega [expr pow($K/$M,0.5)]
# element truss $eleTag $iNode $jNode $A $matTag
  element truss   1        1       2  1.0    1   
 
 
# BOUNDARY CONDITIONS 
fix 1 1 1 
fix 2 0 1 



#filter 1 delta

# MODULATING FUNCTIONS   type     filterTag   arguments
#modulatingFunction  1   constant     1           1

# LOAD PATTERN WITH DISCRETIZED RANDOM PROCESS TIME SERIES
#                                                                         mean                    maxStdev           1stModulatingFunction   2nd...
#pattern UniformExcitation  1   1    -accel {  DiscretizedRandomProcess     0.0                    1.0      1     }

set accelSeries "Series -dt 0.3 -filePath RV2.txt -factor 1";
pattern UniformExcitation 1 1 -accel $accelSeries

#pattern UniformExcitation  1   1    -accel {  DiscretizedRandomProcess     0.0                    1.0      1     }


# RECORDER 
recorder Node -file nodeDisp_1.out  -time -node 2 -dof 1  disp
#recorder Node -file nodeForce.out  -time -node 2 -dof 1  disp

#recorder Node -file nodeAccel.out  -time -node 2 -dof 1  accel
recorder Element -file axial.out  -time -element 1 axialForce
recorder Element -file axial.out  -time -element 1 axialDeformation

# STRUCTURAL ANALYSIS MODEL
system BandSPD
constraints Plain
test NormDispIncr 1.0e-16  50 
algorithm Newton
numberer RCM
#integrator Newmark $gamma $beta <$alphaM $betaK $betaKinit $betaKcomm>
#integrator NewmarkWithSensitivity   0.5  0.25  [expr 2*0.02*$omega]  0.0   0.0   0.0  
integrator Newmark   0.5  0.25  [expr 2*0.02*$omega]  0.0   0.0   0.0  
#2% damping ratio
#sensitivityIntegrator -definedAbove
#sensitivityAlgorithm -computeAtEachStep  
analysis Transient

set totalTime    3.0
set numPulses    10
set numTimeSteps [expr $numPulses*10]

set phi0 0.25
# UNCERTAINTY CHARACTERIZATION
set deltat [expr $totalTime/$numPulses]

analyze $numTimeSteps [expr $totalTime/$numTimeSteps] 

#set nodeDisp [expr [nodeDisp 2 1]]
set u21 [nodeDisp 2 1]
set fo [open PerformanceU.txt w] ;

foreach different_content {u21} { 
puts $fo [set $different_content] 
} 

close $fo 