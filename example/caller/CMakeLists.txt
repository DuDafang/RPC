set(SRC_LIST calluserservice.cc ../user.pb.cc)

add_executable(consumer ${SRC_LIST})
#provider编译的时候需要用到框架的库，生成的provider需要链接框架的动态库
target_link_libraries(consumer mprpc protobuf pthread)