cmake_minimum_required(VERSION 3.5)
project(TikiTest)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

include_directories(${CMAKE_SOURCE_DIR}/product)
include_directories(${CMAKE_SOURCE_DIR}/engine)
include_directories(${CMAKE_SOURCE_DIR}/user)
include_directories(${CMAKE_SOURCE_DIR}/rule)

set(SOURCE_FILES main.cpp product/CProductManager.cpp engine/CEngine.cpp user/CUser.cpp user/CUserManager.cpp rule/CRuleManager.cpp)
add_executable(TikiTest ${SOURCE_FILES})
target_link_libraries(TikiTest ${JSONCPP_LIBRARIES})