cmake_minimum_required(VERSION 3.5)
project(topic_tools)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp_generic REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)

add_library(topic_tools_node SHARED src/relay.cpp)
target_compile_definitions(topic_tools_node
  PRIVATE "TOPIC_TOOLS_BUILDING_DLL")

ament_target_dependencies(topic_tools_node rclcpp_generic rclcpp rclcpp_components)
rclcpp_components_register_node(topic_tools_node
  PLUGIN "topic_tools::RelayNode"
  EXECUTABLE relay
)

add_library(hz_checker SHARED src/hz_checker.cpp)
target_compile_definitions(hz_checker
  PRIVATE "TOPIC_TOOLS_BUILDING_DLL")


ament_target_dependencies(hz_checker rclcpp_generic rclcpp rclcpp_components)
rclcpp_components_register_node(hz_checker
  PLUGIN "topic_tools::HzCheckerNode"
  EXECUTABLE hz_checker_node
)


add_library(node_list_component SHARED src/node_list.cpp)
target_compile_definitions(node_list_component
  PRIVATE "TOPIC_TOOLS_BUILDING_DLL")

ament_target_dependencies(node_list_component rclcpp_generic rclcpp rclcpp_components)
rclcpp_components_register_node(node_list_component
  PLUGIN "topic_tools::NodeList"
  EXECUTABLE node_list
)


install(
  TARGETS topic_tools_node hz_checker node_list_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

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

ament_package()
