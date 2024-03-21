cmake_minimum_required(VERSION 3.25)
project(webService)

add_definitions(-D_WIN32_WINNT=0x0601)
include_directories(${CMAKE_SOURCE_DIR}/lib)



set(CMAKE_CXX_STANDARD 17)

add_executable(webService  main.cpp reactor/MainReactor.cpp reactor/MainReactor.h reactor/SubReactor.cpp reactor/SubReactor.h util/CustomError.cpp util/CustomError.h Service/ServiceClub.cpp util/Logclub.h)

