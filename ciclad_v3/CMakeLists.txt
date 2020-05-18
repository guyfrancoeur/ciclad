cmake_minimum_required (VERSION 3.13)

project (ciclad_v3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_executable(${PROJECT_NAME} ciclad_v3_main.cpp ciclad_v3_impl.cpp)
add_test(NAME testdb_gen COMMAND "cat ${PROJECT_SOURCE_DIR}/../datasets/testdb_gen.txt | ./${PROJECT_NAME}")
add_test(NAME testdb_gen2 COMMAND "cat ${PROJECT_SOURCE_DIR}/../datasets/testdb_gen2.txt | ./${PROJECT_NAME}")

add_test(NAME gnuplot COMMAND "gnuplot -c ../../gp/test.gp")
message("PROJET NAME : ${PROJECT_NAME}")
message("INITIAL FOLDER : ${PROJECT_BINARY_DIR}")

#install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)

#include(CTest)

#if(BUILD_TESTING)
#  ./ciclad_v3
#  add a title to your line
#endif()
