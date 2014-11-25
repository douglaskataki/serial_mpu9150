cmake_minimum_required(VERSION 2.8.3)
project(serial_mpu)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  std_msgs
  visualization_msgs
)

catkin_package(
  INCLUDE_DIRS 
  CATKIN_DEPENDS
    serial
    std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)


add_executable(serial_node src/serial_node.cpp)
add_executable(mpu_rviz src/mpu_rviz.cpp)

target_link_libraries(serial_node
  ${catkin_LIBRARIES}
)

target_link_libraries(mpu_rviz 
  ${catkin_LIBRARIES}
)

add_dependencies(mpu_rviz serial_node_generate_messages_cpp)

#including c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

