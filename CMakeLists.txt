#设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
project(mprpc)

#生成debug版本，可以进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")#gdb ./provider启动调试

#设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

#设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)


#src框架的代码要向外提供成一个so库，所以
#src包含了mprpc框架所有的相关代码
add_subdirectory(src)#在这个目录里面再去搜索CMakeLists

#example包含了mprpc框架使用的示例代码
add_subdirectory(example)