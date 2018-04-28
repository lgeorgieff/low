find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(gflags REQUIRED)

include_directories(${LIBMONGOCXX_INCLUDE_DIRS})
include_directories(${LIBBSONCXX_INCLUDE_DIRS})
include_directories(src/config)

add_subdirectory(src)
