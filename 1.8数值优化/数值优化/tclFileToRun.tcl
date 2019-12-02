wipe

model basic -ndm 2 -ndf 2
# Create nodes & add to Domain - command: node nodeId xCrd yCrd

node 1   0.0  0.0
node 2   10.0  0.0 -mass 3.1741e+003 0 0.0
node 3   20.0  0.0 -mass 4.1741e+003 0 0.0
node 4   30.0  0.0 -mass 5.1741e+003 0 0.0

# Set the boundary conditions - command: fix nodeID xResrnt? yRestrnt?
fix 1 1 1 
fix 2 0 1
fix 3 0 1
fix 4 0 1


 
uniaxialMaterial Steel01  1  $DV_fy1  $DV_E1  $DV_b1 
uniaxialMaterial Steel01  2  $DV_fy2  $DV_E2  $DV_b2 


# element truss trussID node1 node2  A     matID
  element truss     1      1     2   0.01    1
  element truss     2      2     3   0.02    2
  element truss     3      3     4   0.02    1

# Set up the acceleration records for Tabas fault normal and fault parallel
  set tabas "Path -filePath tabas.txt -dt 0.02 -factor 9.8"

#                         tag dir         accel series args
pattern UniformExcitation  1   1  -accel      $tabas


constraints Plain
numberer RCM
test NormDispIncr 1.E-4 25  0
algorithm Newton
system BandSPD
integrator Newmark  0.55 0.275625   
analysis Transient
 

recorder Node -file node4.out -time -node 4 -dof 1 2 -precision 16 disp

 
 
set ok [analyze 2000 0.01]

remove recorders


return $ok
