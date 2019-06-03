 

    source getTotalResistingForce.tcl

    proc accept {sock ip port} {
     
        fconfigure $sock -blocking 1 -buffering none ;#line
 
        fileevent $sock readable [list respond $sock]
    }

    proc respond {sock} {
 
        
        if {[eof $sock] || [catch {gets $sock data}]} {
		# end of file or abnormal connection drop
		close $sock
		puts "Close $sock" ;# $echo(addr,$sock)"
	
	} else {
	
		# safe calculator style evaluator
		puts $data
		eval  $data 
		# --- split data----
		
			
  	        global Fx
	        global My
	      
	        getTotalResistingForce  ; # into Fx, Fy, Fz, Mx, My, Mz. 
	  
	        puts $sock  " $Fx, $My"
	 

	        flush $sock
	     

	 
	     return
     }
    }
    
  

   # ---- model ----------
     source model.tcl
     socket -server accept 7272
     vwait forever

 