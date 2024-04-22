cmake_minimum_required(VERSION 3.8)
project(uav_urdf_demo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY urdf
  DESTINATION share/${PROJECT_NAME}
)

add_executable(state_publisher src/state_publisher.cpp)
ament_target_dependencies(
  state_publisher
  geometry_msgs
  sensor_msgs
  rclcpp
  tf2_ros
  tf2
  std_msgs
)

install(
  TARGETS state_publisher
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(command_publisher src/command_publisher.cpp)
ament_target_dependencies(
  command_publisher
  rclcpp
  std_msgs
)

install(
  TARGETS command_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
