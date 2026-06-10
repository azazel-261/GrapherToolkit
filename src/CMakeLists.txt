add_subdirectory(debugging)
add_subdirectory(dynamicMath)

add_executable(app main.cc mainWindow.cc)

target_link_libraries(app PRIVATE dynMath debugging)