
# F = (u1-u1_0)^2+(u2-u2_0)^2+....
# F_grad = [ 2*((u1-u1_0))* du1/dsita1+2*((u2-u2_0))* du2/dsita1+...,2*((u1-u1_0))* du1/dsita2+2*((u2-u2_0))* du2/dsita2+...,.....]

#   307460.  2.01e8    0.02


set F 0

set fileId_exp [open "node4_exp.txt" "r"]
set fileId_fem [open "node4.out" "r+"]


while {[gets $fileId_exp    u_exp_line] >= 1} {
   if {[gets $fileId_fem    u_fem_line] >= 1} {

	  set count [scan $u_exp_line "%f  %e  " time_exp u_exp ]
	  if {$count != 2} {
	    error "Error reading input - terminating script"
	    exit;
  	  }

	  set count [scan $u_fem_line "%f  %e  %e" time_fem u_fem u_tmp]   ; # u_tmp is u_2_y and useless
	  if {$count != 3} {
	    error "Error reading input - terminating script"
	    exit;
  	  }
  	set F [expr $F + ($u_exp-$u_fem)*($u_exp-$u_fem)];
  }; #if
  
}; #wihle


# clear file

close $fileId_exp
close $fileId_fem


 
 