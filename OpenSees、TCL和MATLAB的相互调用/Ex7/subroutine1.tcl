  set fileId [open "test.txt" "w"]
  puts -nonewline $fileId "calling tcl OK"
  close $fileId