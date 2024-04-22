cmake_minimum_required(VERSION 3.8)
project(uav_tf2_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

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

add_executable(camera_node src/camera_node.cpp)
ament_target_dependencies(
  camera_node
  geometry_msgs
  rclcpp
)

install(
  TARGETS camera_node
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(lidar_node src/lidar_node.cpp)
ament_target_dependencies(
  lidar_node
  geometry_msgs
  rclcpp
)

install(
  TARGETS lidar_node
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(uav_node src/uav_node.cpp)
ament_target_dependencies(
  uav_node
  geometry_msgs
  rclcpp
)

install(
  TARGETS uav_node
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(camera_tf2_broadcaster src/camera_tf2_broadcaster.cpp)
ament_target_dependencies(
  camera_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2_ros
)

install(
  TARGETS camera_tf2_broadcaster
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(uav_tf2_broadcaster src/uav_tf2_broadcaster.cpp)
ament_target_dependencies(
  uav_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2_ros
)

install(
  TARGETS uav_tf2_broadcaster
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(lidar_tf2_broadcaster src/lidar_tf2_broadcaster.cpp)
ament_target_dependencies(
  lidar_tf2_broadcaster
  geometry_msgs
  rclcpp
  tf2_ros
)

install(
  TARGETS lidar_tf2_broadcaster
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(tf2_listener src/tf2_listener.cpp)
ament_target_dependencies(
  tf2_listener
  geometry_msgs
  rclcpp
  tf2_ros
  tf2
)

install(
  TARGETS tf2_listener
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
