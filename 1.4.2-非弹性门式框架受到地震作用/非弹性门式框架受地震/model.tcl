wipe						                                                      
model basic -ndm 2 -ndf 3				                                                  
if { [file exists output] == 0 } {                                                                         
  file mkdir output
}  
node 1 0.     0.					
node 2 12.80  0
node 3 0.     10.97
node 4 12.80  10.97
fix 1 1 1 1			
fix 2 1 1 1 
uniaxialMaterial  Steel01     2        1.47e4      5.74e6      0.01;		
uniaxialMaterial Elastic      3        4.62e7; 			
section Aggregator            1          3        P        2       Mz;	                               
section       Elastic         2       2.49e7       3.72        1.8413;                                 
geomTransf Linear 1; 	
geomTransf Linear 2; 	
element nonlinearBeamColumn   1      1      3         5         1       1;	
element nonlinearBeamColumn   2      2      4         5         1       1;
element nonlinearBeamColumn   3      3      4         5         2       2;
recorder Node    -file output/disp_34.out        -time   -node  3 4            -dof 1 2 3  disp;					
recorder Node    -file output/reaction_12.out    -time   -node  1 2            -dof 1 2 3  reaction;					
recorder Drift   -file output/drift_1.out        -time   -iNode 1 2 -jNode 3 4 -dof 1      -perpDirn 2 ;				
recorder Element -file output/force_12.out       -time   -ele   1 2                        globalForce;						
recorder Element -file output/foce_3.out         -time   -ele   3                          globalForce;						
recorder Element -file output/forcecolsec_1.out  -time   -ele   1 2            section 1   force;				
recorder Element -file output/defocolsec_1.out   -time   -ele   1 2            section 1   deformation;				
recorder Element -file output/forcecolsec_5.out  -time   -ele   1 2            section 5   force;		
recorder Element -file output/defocolsec_5.out   -time   -ele   1 2            section 5   deformation;		
recorder Element -file output/forcebeamsec_1.out -time   -ele   3              section 1   force;				
recorder Element -file output/defobeamsec_1.out  -time   -ele   3              section 1   deformation;				
recorder Element -file output/forcebeamsec_5.out -time   -ele   3              section 5   force;		
recorder Element -file output/defobeamsec_5.out  -time   -ele   3              section 5   deformation;	
pattern Plain 1 Linear {
   eleLoad -ele 3 -type -beamUniform -122.5;
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








mass 3 80. 0. 0.
mass 4 80. 0. 0.
loadConst -time 0.0
timeSeries Path  1 -dt 0.02 -filePath  tabas.txt  -factor 9.8;                
pattern UniformExcitation 2 1  -accel 1;		                            
set temp [eigen 1]
scan $temp "%e"  w1s
set w1 [expr sqrt($w1s)]
puts "第一阶频率   f: [expr $w1/6.28]"
set ksi  0.02
set a0 0
set a1 [expr $ksi*2.0/$w1] 
rayleigh $a0 0.0 $a1  0.0	   
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


