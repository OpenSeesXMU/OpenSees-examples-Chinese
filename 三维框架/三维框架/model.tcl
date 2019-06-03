model BasicBuilder -ndm 3 -ndf 6;
set h 3.6576;                                                                                                  
set by 6.096;                                                                                                  
set bx 6.096;                                                                                                
node  1     [expr -$bx/2]    [expr  $by/2]        0
node  2     [expr  $bx/2]    [expr  $by/2]        0
node  3     [expr  $bx/2]    [expr -$by/2]        0 
node  4     [expr -$bx/2]    [expr -$by/2]        0 
node  5     [expr -$bx/2]    [expr  $by/2]        $h 
node  6     [expr  $bx/2]    [expr  $by/2]        $h 
node  7     [expr  $bx/2]    [expr -$by/2]        $h 
node  8     [expr -$bx/2]    [expr -$by/2]        $h 
node 10     [expr -$bx/2]    [expr  $by/2]    [expr 2*$h]
node 11     [expr  $bx/2]    [expr  $by/2]    [expr 2*$h] 
node 12     [expr  $bx/2]    [expr -$by/2]    [expr 2*$h] 
node 13     [expr -$bx/2]    [expr -$by/2]    [expr 2*$h] 
node 15     [expr -$bx/2]    [expr  $by/2]    [expr 3*$h] 
node 16     [expr  $bx/2]    [expr  $by/2]    [expr 3*$h] 
node 17     [expr  $bx/2]    [expr -$by/2]    [expr 3*$h] 
node 18     [expr -$bx/2]    [expr -$by/2]    [expr 3*$h]
node  9            0                0             $h 
node 14            0                0         [expr 2*$h]
node 19            0                0         [expr 3*$h]
fix  1   1  1  1  1  1  1
fix  2   1  1  1  1  1  1
fix  3   1  1  1  1  1  1
fix  4   1  1  1  1  1  1
rigidDiaphragm      3          9        5     6     7     8
rigidDiaphragm      3         14       10    11    12    13
rigidDiaphragm      3         19       15    16    17    18
fix  9   0  0  1  1  1  0
fix 14   0  0  1  1  1  0
fix 19   0  0  1  1  1  0
uniaxialMaterial Concrete01     1     -34473.8    -0.005     -24131.66     -0.02
set fc 27579.04
uniaxialMaterial Concrete01    2        -$fc      -0.002        0.0        -0.006
uniaxialMaterial Steel01       3       248200.     2.1e8        0.02
set E 24855585.89304;
set GJ 68947600000000;
uniaxialMaterial Elastic  10      $GJ
set d 0.4572
source RCsection.tcl
RCsection   1  $d  $d     0.04        1      2        3       3     0.00051       8       8       10       10
section Aggregator            2        10      T    -section    1
set colSec 2  
geomTransf Linear   1        1  0  0
set np 4
element dispBeamColumn    1    1     5       $np      $colSec    1
element dispBeamColumn    2    2     6       $np      $colSec    1
element dispBeamColumn    3    3     7       $np      $colSec    1
element dispBeamColumn    4    4     8       $np      $colSec    1
element dispBeamColumn    5    5    10       $np      $colSec    1
element dispBeamColumn    6    6    11       $np      $colSec    1
element dispBeamColumn    7    7    12       $np      $colSec    1
element dispBeamColumn    8    8    13       $np      $colSec    1
element dispBeamColumn    9   10    15       $np      $colSec    1
element dispBeamColumn   10   11    16       $np      $colSec    1
element dispBeamColumn   11   12    17       $np      $colSec    1
element dispBeamColumn   12   13    18       $np      $colSec    1
set Abeam 0.278709
set Ibeamzz 0.004315;
set Ibeamyy 0.002427;     
section Elastic    3     $E  $Abeam  $Ibeamzz     $Ibeamyy          $GJ        1.0                       
set beamSec 3    
geomTransf Linear   2         1 1 0
set np 3              
element dispBeamColumn   13     5      6     $np      $beamSec     2
element dispBeamColumn   14     6      7     $np      $beamSec     2
element dispBeamColumn   15     7      8     $np      $beamSec     2
element dispBeamColumn   16     8      5     $np      $beamSec     2
element dispBeamColumn   17    10     11     $np      $beamSec     2
element dispBeamColumn   18    11     12     $np      $beamSec     2
element dispBeamColumn   19    12     13     $np      $beamSec     2
element dispBeamColumn   20    13     10     $np      $beamSec     2
element dispBeamColumn   21    15     16     $np      $beamSec     2
element dispBeamColumn   22    16     17     $np      $beamSec     2
element dispBeamColumn   23    17     18     $np      $beamSec     2
element dispBeamColumn   24    18     15     $np      $beamSec     2
set p  74.0
set g 9.8;            
set m 30.0;
set i [expr $m*($bx*$bx+$by*$by)/12.0]
mass  9  $m $m  0  0  0 $i
mass 14  $m $m  0  0  0 $i
mass 19  $m $m  0  0  0 $i
pattern Plain 1 {Series -time {0.0 2.0 100000.0} -values {0.0 1.0 1.0} } {          
   foreach node {5 6 7 8  10 11 12 13  15 16 17 18} {
      load $node 0.0 0.0 -$p 0.0 0.0 0.0
   }
}
set tabasFN "Path -filePath tabasFN.txt -dt 0.02 -factor $g"
set tabasFP "Path -filePath tabasFP.txt -dt 0.02 -factor $g"
pattern UniformExcitation  2      1   -accel      $tabasFN
pattern UniformExcitation  3      2   -accel      $tabasFP
recorder Node -file node.out -time -node 9 14 19 -dof 1 2 3 4 5 6 disp
constraints Transformation                        
test EnergyIncr 1.0e-16  20  2
integrator LoadControl 1 1 1 1
algorithm Newton
system BandGeneral
numberer RCM
analysis Static 
set startT [clock seconds]
analyze 3
puts "三层框架结构非线性分析完成..."

wipeAnalysis
test EnergyIncr 1.0e-16 20  2
algorithm Newton
system BandGeneral
constraints Transformation
numberer RCM
integrator Newmark   0.55 0.275625   
analysis Transient
analyze   2500   0.01
set endT [clock seconds]
puts "完成时间: [expr $endT-$startT] seconds."