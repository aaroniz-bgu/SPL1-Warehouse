#cmake_minimum_required(VERSION 3.27)
project(assignment_1)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(assignment_1
        include/Action.h
        include/Customer.h
        include/Order.h
        include/Volunteer.h
        include/WareHouse.h
        src/main.cpp
        src/Customer.cpp
        src/Order.cpp
        src/WareHouse.cpp
        src/Volunteer.cpp
        src/Action.cpp
        src/ActionFactory.cpp
        include/ActionFactory.h)
