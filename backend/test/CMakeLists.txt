if(GENERATE_TESTS)
  find_package(GTest REQUIRED)

  add_executable(config_unit_tests
    config/config_factory.cpp
    config/json_reader.cpp
    config/config_error.cpp
    config/http_config.cpp
    config/mongodb_config.cpp

    ../src/config/config_error.cpp
    )
  target_link_libraries(config_unit_tests ${GTEST_BOTH_LIBRARIES})
  add_test(NAME config COMMAND config_unit_tests)

  include_directories(${GTEST_INCLUDE_DIRS})
endif()
