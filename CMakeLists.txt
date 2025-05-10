cmake_minimum_required(VERSION 3.10.0)
project(Coursework VERSION 0.1.0 LANGUAGES C CXX)

add_library(Coursework directed_graph.h directed_graph.cpp hash_table.hpp linked_list.hpp queue.hpp)


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Находим все тестовые файлы
file(GLOB TEST_SOURCES "tests/*.cpp")

# Создаем отдельный тест для каждого исходного файла
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    
    add_executable(${test_name} 
        ${test_source}
        directed_graph.cpp
        directed_graph.h
        linked_list.hpp
        hash_table.hpp
        queue.hpp
    )
    
    target_link_libraries(${test_name}
        PRIVATE 
        GTest::gtest_main
    )
    
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endforeach()

enable_testing()