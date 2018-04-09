add_executable(low_service src/http/main.cpp)
TARGET_LINK_LIBRARIES(low_service proxygenhttpserver folly glog gflags pthread)
