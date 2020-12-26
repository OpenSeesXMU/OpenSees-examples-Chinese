#file delete Performance.txt
model basic -ndm 2 -ndf 3 
node 1 0.0 0.0 
node 2 0.0 6000.0 

fix 1 1 1 1
fix 2 0 1 1

geomTransf Linear 1

element elasticBeamColumn  1  1  2 562500 3.0E+04 2.636719E+010 1

set F_channel [open RV.txt "r"];
set FData [read $F_channel];
close $F_channel;

set F [lindex $FData 0];

pattern Plain 1 Linear {
load 2 $F 0.0 0.0
}

recorder Node -file DisplacementUnderDeadLoad.txt -time -node 2 -dof 1 disp

constraints Lagrange
numberer Plain
system BandGeneral
test EnergyIncr 1.0e-006 200
algorithm Newton
integrator LoadControl 0.5
analysis Static
analyze 2


#set nodeDisp [expr [nodeDisp 2 1]]
set u21 [nodeDisp 2 1]
set fo [open PerformanceU.txt w] ;

foreach different_content {u21} { 
puts $fo [set $different_content] 
} 

close $fo 