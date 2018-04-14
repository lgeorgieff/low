add_executable(low_service src/http/main.cpp)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
include_directories(${LIBBSONCXX_INCLUDE_DIRS})

target_link_libraries(low_service proxygenhttpserver folly glog gflags pthread)
target_link_libraries(low_service ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(low_service ${LIBBSONCXX_LIBRARIES})
