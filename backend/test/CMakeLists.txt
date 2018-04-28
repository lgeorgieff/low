if(GENERATE_TESTS)
  add_executable(config_unit_tests
    config/config_factory.cpp
    config/json_reader.cpp
    config/config_error.cpp
    config/http_config.cpp
    config/mongodb_config.cpp
    )
  add_test(NAME config_unit_tests COMMAND config_unit_tests)

  find_package(GTest REQUIRED)

  include_directories(${GTEST_INCLUDE_DIRS})

  target_link_libraries(config_unit_tests ${GTEST_BOTH_LIBRARIES})
endif()
