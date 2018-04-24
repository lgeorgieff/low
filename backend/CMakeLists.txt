add_executable(low_service
  src/main.cpp

  src/config/config_factory.cpp
  src/config/http_config.cpp
  src/config/mongodb_config.cpp
  src/config/config_error.cpp
  src/config/json_reader.cpp
  )

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(gflags REQUIRED)

include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
include_directories(${LIBBSONCXX_INCLUDE_DIRS})
include_directories(src/config)

target_link_libraries(low_service proxygenhttpserver)
target_link_libraries(low_service folly)
target_link_libraries(low_service glog)
target_link_libraries(low_service gflags)
target_link_libraries(low_service pthread)

target_link_libraries(low_service jsoncpp)

target_link_libraries(low_service ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(low_service ${LIBBSONCXX_LIBRARIES})

target_link_libraries(low_service gflags)
