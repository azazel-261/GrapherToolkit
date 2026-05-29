cmake_minimum_required(VERSION 4.0)
project(GrapherToolkit)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

target_include_directories(app PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${GTKMM_LIBRARIES})
