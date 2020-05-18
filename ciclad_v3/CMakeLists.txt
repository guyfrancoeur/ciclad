cmake_minimum_required (VERSION 3.13)

project (ciclad_v3)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_executable(${PROJECT_NAME} ciclad_v3_main.cpp ciclad_v3_impl.cpp)
add_test(NAME test1 COMMAND cat ../datasets/testdb_gen.txt | ./bin/${PROJECT_NAME})
#add_test(NAME test2 COMMAND ${PROJECT_NAME})

#include(CTest)

#if(BUILD_TESTING)
#  ./ciclad_v3
#  add a title to your line
#endif()
