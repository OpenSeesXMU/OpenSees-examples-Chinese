# CMake minimum required VERSION
cmake_minimum_required (VERSION 3.18)
project (ORS)

# add the header files directory of Tcl
include_directories(C:/Tcl/include)
# add the header files directory of Reliability
include_directories(D:/Cmake/test43)

# add the libaraly files directory of Tcl
link_directories("C:/Tcl/lib")
# add the libaraly files directory of vcf2c2
link_directories("D:/Cmake/test43")

#Set the name of the library files to be linked
LINK_LIBRARIES(tcl84 vcf2c2 tcl85 tclstub84 tk84 tkstub84 tclstub85 tk85 tkstub85)

# Find all source files in the specified directory
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(ORS ${DIR_SRCS})
