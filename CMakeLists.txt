cmake_minimum_required(VERSION 3.27)
project(assignment_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(assignment_1
        include/Action.h
        include/Customer.h
        include/Order.h
        include/Volunteer.h
        include/WareHouse.h
        src/main.cpp
        src/Customer.cpp
        src/Order.cpp)
