cmake_minimum_required(VERSION 3.10.2)
project(status)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -L/usr/local/lib -Wl,-rpath=/usr/local/lib")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -L/usr/local/lib -Wl,-rpath=/usr/local/lib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static_lib)

#tp
set(YAML_TP_PATH tp/yaml-cpp)
add_subdirectory(${YAML_TP_PATH})

#exec
file(GLOB sources "src/[a-zA-Z]*.cpp")
file(GLOB private_headers "src/[a-zA-Z]*.h")

add_executable(${PROJECT_NAME}
        ${sources}
        ${private_headers})

target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${YAML_TP_PATH}/include)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("BUILD CMAKE IN DEBUG MODE")
    target_compile_definitions(fastlog PUBLIC LOG_VERBOSE)
    target_compile_options(${PROJECT_NAME} PUBLIC -W -Wall -pedantic -O0 -g)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_VERSION USE_LOCAL_IP)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(fastlog PUBLIC LOG_VERBOSE)
    message("BUILD CMAKE IN RELEASE MODE")
    target_compile_options(${PROJECT_NAME} PUBLIC -W -Wall -pedantic -O2)
endif ()

target_link_libraries(${PROJECT_NAME}
        yaml-cpp
        fastlog
        adapter
        application
)