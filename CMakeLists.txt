cmake_minimum_required(VERSION 3.26)
project(tks_status_server)

set(CMAKE_CXX_STANDARD 17)

#Modules
add_subdirectory(fastlog)
add_subdirectory(etcdregistry)

add_subdirectory(application)
add_subdirectory(adapter)
add_subdirectory(bootstrap)
