aux_source_directory(. SRC_LIST)#当前目录的所有源文件名字放进SRC_LIST
#set(SRC_LIST mprpcapplication.cc mprpcconfig.cc)
add_library(mprpc SHARED ${SRC_LIST})#编译成library，由SRC_LIST这些源文件生成一个shared动态库