proc getTotalResistingForce {} {


global Me
global H
global L


global Fx
global My

set Fx 0
set My 0


set Fx1 [nodeReaction 1 1]
set Fx2 [nodeReaction 4 1]
set Fx [expr $Fx1+$Fx2]
set Fy1 [nodeReaction 1 2]
set Fy2 [nodeReaction 4 2]
set My1 [nodeReaction 1 3]
set My2 [nodeReaction 4 3]


set My [expr $Fy1*$L/2-$Fy2*$L/2-$My1-$My2]


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