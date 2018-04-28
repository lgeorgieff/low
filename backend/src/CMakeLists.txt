add_executable(low_service
  main.cpp

  config/config_factory.cpp
  config/http_config.cpp
  config/mongodb_config.cpp
  config/config_error.cpp
  config/json_reader.cpp
  )

target_link_libraries(low_service proxygenhttpserver)
target_link_libraries(low_service folly)
target_link_libraries(low_service glog)
target_link_libraries(low_service gflags)
target_link_libraries(low_service pthread)

target_link_libraries(low_service jsoncpp)

target_link_libraries(low_service ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(low_service ${LIBBSONCXX_LIBRARIES})
