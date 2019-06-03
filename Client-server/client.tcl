set s [socket localhost 7272]; fconfigure $s -buffering none;


 
puts $s "runOpenSeesOneStep 1 0.01 0.005  0.0002;"
set a [gets $s]

puts $s "tryOpenSeesOneStep 1 0.01 0.005  0.0002;"
set a [gets $s]

puts $s "commitOpenSeesOneStep"
set a [gets $s]

puts $s "runOpenSeesOneStep 2 0.01 0.005  0.0002;"
set a [gets $s]
	