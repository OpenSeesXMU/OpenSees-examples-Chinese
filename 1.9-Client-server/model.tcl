
#   unit. meter, N, Pa,Kg
#
#
#
#                             2'             
#                2===========================3                  =| 36.3
#                ||                         ||                   |
#                ||                         ||                   |
#                ||                         ||                   |
#                ||                         ||                    
#                ||                         ||                   |
#                || 1'                      || 3'                |
#                ||                         ||                   |
#                ||                         ||                   |
#                ||                         ||                   |
#                1                           4                  =| 0.0
                                                        
#                                                                 
#               
#    
#
#
#       
#________________________________________________________________
#                 |            |            |                      
#                0.0         12.6          25.2
#  
#


#  unit:  m, s, kN, k-kg.


# CREATE THE MODEL BUILDERS FOR UPGROUND FRAMES
model BasicBuilder -ndm 2 -ndf 3


# ------------------- DEFINE beam NODES  ---------------------------------
global Me
set Me [expr 7.55e6/2.0]


global H
global L


set H 36.3
set L 25.2

#    tag  X      Y     
node  1   0.0   0.0     
node  2   0.0   $H   -mass $Me $Me 0.0
node  4   $L    0.0   
node  3   $L    $H   -mass $Me $Me 0.0 


#fix 1   1   1   1
#fix 4   1   1   1

# GEOMETRIC TRANSFORMATION
geomTransf Linear 1

set Area [expr 6.1*6.1]
set Iz   [expr 6.1*6.1*6.1*6.1/12.0]

set factorK 1.0

# ---------------element 
#element elasticBeamColumn $eleTag $iNode $jNode $A     $E      $Iz      $transfTag
element elasticBeamColumn       1    1       2   $Area  [expr 2.7e10*$factorK]   $Iz      1
element elasticBeamColumn       2    2       3   $Area  [expr 2.0e14*$factorK]   $Iz      1
element elasticBeamColumn       3    3       4   $Area  [expr 2.7e10*$factorK]   $Iz      1

constraints Transformation
test NormDispIncr 1.E-6 25  1
algorithm Newton
numberer RCM
system BandGeneral


# set  w1 41.9453692319 ;  set  w2 256.874619221; #   --- factorK = 9
#set w1 88.428592661;
#set  w2 541.539195257  ; #--- factorK = 40
# w1: 19.7732344345 , w2: 121.091865953  --- factorK = 2

set w1 13.9817881546 ; 
set w2 85.6248678831  ; # ---- factorK = 1

set ksi  0.02
set a0 [expr $ksi*2.0*$w1*$w2/($w1+$w2)]
set a1 [expr $ksi*2.0/($w1+$w2)]

#  integrator Newmark $gamma       $beta          
integrator Newmark        0.5       0.25  
#  rayleigh  <$alphaM         $betaK             $betaKinit $betaKcomm>  
rayleigh $a0   0.0 $a1   0.0        
analysis Transient

recorder Node -file disp.out -time -node 2 3 -dof 1 2 3 disp
recorder Node -file reaction.out -time -node 1 4 -dof 1 2 3 reaction


