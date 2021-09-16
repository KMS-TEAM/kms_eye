add_executable(sp_feature_test main.cpp)

target_link_libraries(sp_feature_test  
                        SpFeature
                        OpenMP::OpenMP_CXX
                        ${TORCH_LIBRARIES}
                        ${OpenCV_LIBS}
                        glog
                        gflags})