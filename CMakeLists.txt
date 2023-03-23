#cmake_minimum_required(3.18)

project(chat28cpp)

set(SRC_FILES main.cpp tiarray.cpp CUser.cpp CSession.cpp CMessage.cpp)

add_executable(chat28cpp ${SRC_FILES})