cmake_minimum_required(VERSION 3.16)
project(Database)

set(CMAKE_CXX_STANDARD 14)

add_executable(Database sources/main.cpp sources/CommandParser.cpp sources/Table.cpp headers/Table.h sources/Catalog.cpp headers/Catalog.h)