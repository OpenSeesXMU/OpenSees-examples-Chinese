proc getTotalResistingForce {} {


global Me
global H
global L


global Fx
global My

set Fx 0
set My 0




    # scan $theAccel "%f   %e  %e  %e   %e"  time accel_2_x  accel_2_y   accel_3_x  accel_3_y
     
    set accel_2_x [nodeAccel 2 1]
    set accel_2_y [nodeAccel 2 2] 
    set accel_3_x [nodeAccel 3 1]
    set accel_3_y [nodeAccel 3 2] 

     
#  注意： 新版本中有直接记录节点反力的命令，下面计算可被简化！
#  note this accel_x is already absolute one.
		set Fx [expr $Fx+$Me*$accel_2_x+$Me*$accel_3_x ]
		set My [expr $My-$Me*$accel_2_x*$H-$Me*$accel_3_x*$H -$Me*$accel_2_y*$L/2.0+$Me*$accel_3_y*$L/2.0 ]


}




proc runOpenSeesOneStep { numOfTimeStep   timeStep  currentDisp_1  currentDisp_2}  {
 
          global L

          
          remove  loadPattern 1   ; # correponding to node 1 dof 1

          pattern MultipleSupport 1 {
		groundMotion 11 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_1}" 
		imposedMotion   1      1      11

            set currentDisp_y [expr -$currentDisp_2*$L/2 ] 
		groundMotion 12 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep]  } -values { $currentDisp_y  } " 
		imposedMotion   1      2      12

		groundMotion 13 Plain -disp   "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_2}"  
		imposedMotion   1      3      13

		groundMotion 14 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_1}" 
		imposedMotion   4      1      14

            set currentDisp_y [expr $currentDisp_2*$L/2 ]
		groundMotion 15 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_y}"  
		imposedMotion   4      2      15

		groundMotion 16 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_2}"  
		imposedMotion   4      3      16
           }

 

 
        analyze 1 [expr $timeStep]      
        
 

     } 




proc tryOpenSeesOneStep { numOfTimeStep   timeStep  currentDisp_1  currentDisp_2}  {
 
          global L

          
          remove  loadPattern 1   ; # correponding to node 1 dof 1

          pattern MultipleSupport 1 {
		groundMotion 11 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_1}" 
		imposedMotion   1      1      11

            set currentDisp_y [expr -$currentDisp_2*$L/2 ] 
		groundMotion 12 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep]  } -values { $currentDisp_y  } " 
		imposedMotion   1      2      12

		groundMotion 13 Plain -disp   "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_2}"  
		imposedMotion   1      3      13

		groundMotion 14 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_1}" 
		imposedMotion   4      1      14

            set currentDisp_y [expr $currentDisp_2*$L/2 ]
		groundMotion 15 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_y}"  
		imposedMotion   4      2      15

		groundMotion 16 Plain -disp  "Series -time { [expr $numOfTimeStep*$timeStep] } -values {  $currentDisp_2}"  
		imposedMotion   4      3      16
           }

 

 
        tryOneStep [expr $timeStep]      

     } 


 proc commitOpenSeesOneStep {}  {
	commitOneStep;
       

 

 }