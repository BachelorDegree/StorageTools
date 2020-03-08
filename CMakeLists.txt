CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(StorageTools)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "-Wall -fPIC")

INCLUDE_DIRECTORIES("CoreDeps/include")

ADD_EXECUTABLE(init_disk init_disk/main.cpp)
ADD_EXECUTABLE(check_disk check_disk/main.cpp)