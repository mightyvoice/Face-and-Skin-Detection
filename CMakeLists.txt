cmake_minimum_required(VERSION 2.8)
project( SkinDetection )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )

######### add jsoncpp static library ################
add_library(jsoncppLib STATIC IMPORTED)
#set_property(TARGET jsoncppLib PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libjsoncpp.a)
set_property(TARGET jsoncppLib PROPERTY IMPORTED_LOCATION ./jsoncpp-master/build/debug/libjsoncpp.a)
######### add jsoncpp static library ################

add_executable( SkinDetection ./src/main.cpp ./src/AdaptiveSkinDetector.cpp ./src/Histogram.cpp ./src/FaceDetection.cpp ./src/GetAllBodySkinRatios.cpp ./src/ParseJson.cpp)
target_link_libraries( SkinDetection ${OpenCV_LIBS} jsoncppLib)
