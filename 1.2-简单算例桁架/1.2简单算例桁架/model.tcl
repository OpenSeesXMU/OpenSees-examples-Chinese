  wipe;
  model Basic -ndm 2 -ndf 2
  if { [file exists output] == 0 } {
    file mkdir output;
  }
 node 1 0.0 0.0
 node 2 144.0 0.0
 node 3 168.0 0.0
 node 4 72.0 96.0
 fix 1 1 1
 fix 2 1 1
 fix 3 1 1
 uniaxialMaterial Elastic 1 3000.0
 element truss 1 1 4 10.0 1
 element truss 2 2 4 5.0 1
 element truss 3 3 4 5.0 1
 recorder Node -file output/disp_4.out -time -node 4 -dof 1 2 disp
 recorder Node -file output/reaction_1.out -time -node 1 -dof 1 2 reaction
 recorder Node -file output/reaction_2.out -time -node 2 -dof 1 2 reaction
 recorder Node -file output/reaction_3.out -time -node 3 -dof 1 2 reaction
 pattern Plain 1 Linear {
   load 4 100.0 -50.0
 }
 constraints Transformation
 numberer RCM
 system BandSPD
 test NormDispIncr 1.0e-6 6 2
 algorithm Newton
 integrator LoadControl 0.1
 analysis Static
 analyze 10
 wipeAnalysis
 loadConst -time 0.0
 mass 4 100.0 100.0 
 pattern UniformExcitation  2   1  -accel  "Series -factor 3 -filePath elcentro.txt -dt 0.01"
 constraints Transformation
 numberer RCM
 system BandSPD
 test NormDispIncr 1.0e-6 6 4
 algorithm Newton
 integrator Newmark 0.5 0.25
 analysis Transient
 analyze 2000 0.01