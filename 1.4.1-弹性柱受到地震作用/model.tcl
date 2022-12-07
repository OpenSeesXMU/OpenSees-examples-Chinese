wipe;						                                                           
model basic -ndm 2 -ndf 3;				                                                
if { [file exists output] == 0 } {                                                   
  file mkdir output;
}  
node 1 0 0;					                                                           
node 2 0 3.0;
node 3 0 6.0;
fix 1 1 1 1; 			                                                               
geomTransf Linear 1;		                                                                          
element elasticBeamColumn    1      1      2       0.25   3.0e10   5.2e-3      1;	
element elasticBeamColumn    2      2      3       0.25   3.0e10   5.2e-3      1;	
recorder Node      -file   output/disp_3.out        -time          -node 3    -dof 1 2 3      disp;	
recorder Node      -file   output/disp_2.out        -time          -node 2    -dof 1 2 3      disp;		   
recorder Node      -file   output/reaction_1.out    -time          -node 1    -dof 1 2 3      reaction;			  
recorder Drift     -file   output/drift_1.out       -time    -iNode 1 -jNode 2  -dof 1       -perpDirn 2 ;		  
recorder Drift     -file   output/drift_2.out       -time    -iNode 2 -jNode 3  -dof 1       -perpDirn 2 ;		  
recorder Element   -file   output/force_1.out       -time          -ele 1                     globalForce;	           
pattern Plain 1 Linear {
   load   2      0.       -1.0e5      0.;
   load   3      0.       -1.0e5      0.;
}
constraints Plain;     				                                                          
numberer Plain;					                                                             
system BandGeneral;				                                                              
test NormDispIncr 1.0e-8 6 2; 			                                                      
algorithm Newton;					                                                         
integrator LoadControl 0.1;				                                                      
analysis Static					                                                              
analyze 10;					                                                              
puts "重力分析完成..."








mass 2 1.0e4 0 0;                                                                                         
mass 3 1.0e4 0 0;
loadConst -time 0.0;				
timeSeries Path        1      -dt    0.02      -filePath   tabas.txt    -factor 9.8;
pattern UniformExcitation   2             1         -accel          1;
set temp [eigen 1]
scan $temp "%e"  w1s
set w1 [expr sqrt($w1s)]
puts "第一阶频率   f: [expr $w1/6.28]"
set ksi  0.02
set a0 0
set a1 [expr $ksi*2.0/$w1]
rayleigh          $a0            0.0            $a1                0.0
wipeAnalysis;					                                                              
constraints Plain;     				                       
numberer Plain;					                       
system BandGeneral;				
test NormDispIncr 1.0e-8 10 2;			
algorithm Newton;					
integrator Newmark 0.5 0.25 ;			                                                               
analysis Transient;					                                                       
analyze 1000 0.02;					                                                      
puts 地面运动分析完成..."



