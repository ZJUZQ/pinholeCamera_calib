cmake_minimum_required(VERSION 2.8)
project( pinholeCamera_calibration )
#add_compile_options(-std=c++11)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( camera_calibration src/camera_calibration.cpp )
target_link_libraries( camera_calibration ${OpenCV_LIBS} )

add_executable( usbCamera_save src/usbCamera_save.cpp )
target_link_libraries( usbCamera_save ${OpenCV_LIBS} )
