add_executable(visual_odom_test main.cpp)

target_link_libraries(visual_odom_test  visual_odom ${OpenCV_LIBS})