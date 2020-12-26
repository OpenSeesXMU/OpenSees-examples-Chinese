#file delete Performance.txt
model basic -ndm 2 -ndf 2 
#  reliability          
#读取地震波峰值PGA
set GMScale_sample [open RV.txt "r"];
set GMScale_data [read $GMScale_sample];
close $GMScale_sample;

 set E [lindex $GMScale_data 0];
 set fy [lindex $GMScale_data 1];
 set P [lindex $GMScale_data 2];

node 1 0.0    0.0 
node 2 5000.0 0.0 

# set  E   210000.0
# set  fy  355.0
# set  P   337500.0

uniaxialMaterial Steel01  1  $fy  $E  0.02
#uniaxialMaterial SmoothSteel01  1  $fy  $E  0.02 0.5 10.0

element truss 1  1 2 1000.0  1

fix 1 1 1 
fix 2 0 1

pattern Plain 1 Linear { 
load 2 $P 0.0 
} 

recorder Node nodeDisp.out -time -node 2 -dof 1 disp

constraints Plain
numberer RCM
test NormUnbalance 1.0e-6 25 0
integrator LoadControl 0.025 1 0.025 0.025
algorithm Newton
system ProfileSPD
#sensitivityIntegrator -static
#sensitivityAlgorithm -computeAtEachStep 
integrator LoadControl  0.02   3   0.02    0.02
analysis Static 
analyze 10

#set nodeDisp [expr [nodeDisp 2 1]]
set u21 [nodeDisp 2 1]
set fo [open PerformanceU.txt w] ;

foreach different_content {u21} { 
puts $fo [set $different_content] 
} 

close $fo 