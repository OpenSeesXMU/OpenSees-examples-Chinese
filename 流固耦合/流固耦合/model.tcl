wipe;
model basic -ndm 3 -ndf 3
node 1 1.00000 0.0000  0.0000
node 2 1.00000 1.0000  0.0000
node 3 0.00000 1.0000  0.0000
node 4 0.00000 0.0000  0.0000
node 5 1.00000 0.0000  1.0000
node 6 1.00000 1.0000  1.0000
node 7 0.00000 1.00000 1.0000
node 8 0.00000 0.00000 1.0000
nDMaterial TruncatedDP 1 3 2400000 1.4957e+010 1.7677e+010 0.11 2.6614e7 -2.0684e6 1.0e-8
set g 9.8
set rhog [expr -2400000*$g]
element bbarBrick 1 1 2 3 4 5 6 7 8 1 0 0 $rhog
recorder Element -ele 1 -time -file stress1.out -time material 1 stress
recorder Element -ele 1 -time -file stress2.out -time material 2 stress
recorder Element -ele 1 -time -file stress3.out -time material 3 stress
recorder Element -ele 1 -time -file stress4.out -time material 4 stress
recorder Element -ele 1 -time -file stress5.out -time material 5 stress
recorder Element -ele 1 -time -file stress6.out -time material 6 stress
recorder Element -ele 1 -time -file stress7.out -time material 7 stress
recorder Element -ele 1 -time -file stress8.out -time material 8 stress
recorder Element -ele 1 -time -file strain1.out -time material 1 strain
recorder Element -ele 1 -time -file strain2.out -time material 2 strain
recorder Element -ele 1 -time -file strain3.out -time material 3 strain
recorder Element -ele 1 -time -file strain4.out -time material 4 strain
recorder Element -ele 1 -time -file strain5.out -time material 5 strain
recorder Element -ele 1 -time -file strain6.out -time material 6 strain
recorder Element -ele 1 -time -file strain7.out -time material 7 strain
recorder Element -ele 1 -time -file strain8.out -time material 8 strain
set E 2.5E10
set G 10E10
set rho 2600
set R 475
set alphaN 1.33
set alphaT 0.67
element VS3D4 5 3 4 8 7 $E $G $rho $R $alphaN $alphaT
element VS3D4 6 1 2 6 5 $E $G $rho $R $alphaN $alphaT
element VS3D4 7 1 2 3 4 $E $G $rho $R $alphaN $alphaT
element VS3D4 8 1 5 8 4 $E $G $rho $R $alphaN $alphaT
recorder Node -file node5.out -time -precision 16 -node 5 -dof 1 disp
recorder Node -file node6.out -time -precision 16 -node 6 -dof 1 disp


model basic -ndm 3 -ndf 1
node 9 1.00000 1.0000 0.00000
node 10 1.00000 2.0000 0.0000
node 11 0.00000 2.0000 0.000000
node 12 0.00000 1.0000 0.00000
node 13 1.00000 1.0000 1.0000
node 14 1.00000 2.0000 1.000000
node 15 0.00000 2.00000 1.00000
node 16 0.00000 1.00000 1.0000
set water 2
nDMaterial AcousticMedium $water 2.069857690E+09 1000
element AC3D8 3 9 10 11 12 13 14 15 16 $water
element ASI3D8 2 2 3 6 7 9 12 13 16
element AV3D4 4 10 11 15 14 $water
fix 13 1
fix 14 1
fix 15 1
fix 16 1
recorder Node -file node9.out -time -precision 16 -node 9 -dof 1 2 3 disp
recorder Node -file node10.out -time -precision 16 -node 10 -dof 1 disp
puts "Define model acoustic ok...."
timeSeries Path 1 -dt 0.01 -filePath elcentro.txt -factor 3
pattern Plain 111 1 {
load 5 1e6 0 1e6
load 6 1e6 0 1e6
load 7 1e6 0 1e6
load 8 1e6 0 1e6
}
wipeAnalysis
constraints Transformation
system BandGeneral
numberer Plain
test NormDispIncr 1.0E-5 20 2
algorithm Newton
integrator Newmark 0.5 0.25
analysis Transient
set startT [clock seconds]
analyze 1600 0.01
puts "Dynamic analysi done..."
set endT [clock seconds]
puts "Execution time: [expr $endT-$startT] seconds."
