get_filename_component(project_name_ ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(TOLOWER ${project_name_} project_name_)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
project(${project_name_} VERSION 0.0.1)
ADD_DEFINITIONS(-D_PROJECT_VERSION=\"${PROJECT_VERSION}\")

#Debug｜Release模式: -g表示带调试信息，-Wall表示输出所有的警告信息
set(CMAKE_BUILD_TYPE Release) #cmake根据编译器加-g参数(调试)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC_FILE)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(install_dir /usr/local/cc13535)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILE})
else()
    set(install_dir ${CMAKE_SOURCE_DIR}/build)
    add_library(${PROJECT_NAME} STATIC ${SRC_FILE})
endif()

set(CMAKE_PREFIX_PATH ${install_dir}/cmake/modules)
find_package(gzip REQUIRED NO_MODULE)
find_package(mysql REQUIRED NO_MODULE)
find_package(ccore 0.0 REQUIRED NO_MODULE)
find_package(mysqlorm 6.2 REQUIRED NO_MODULE)
include_directories("include")
add_executable(${PROJECT_NAME}_run main.cpp)
add_dependencies(${PROJECT_NAME}_run ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_run -lpthread ccore mysqlorm ${PROJECT_NAME} ${gzip_LIBS} ${mysql_LIBS})