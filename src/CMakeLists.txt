aux_source_directory(. SRC_LIST)#当前目录的所有源文件名字放进SRC_LIST
#set(SRC_LIST mprpcapplication.cc mprpcconfig.cc rpcheader.pb.cc rpcprovider.cc mprpcchannel.cc)#把所有的源文件都放进来，可以防止上一行代码出问题
add_library(mprpc ${SRC_LIST})#编译成library，由SRC_LIST这些源文件生成一个shared动态库
target_link_libraries(mprpc muduo_net muduo_base pthread)#sudo find /usr -name "libmuduo*"
#muduo库是静态库，生成的动态库链接静态库的时候会有问题，所以把我们的库也编成静态库
#此外库和库之间也有先后顺序、链接关系，所以先写muduo_net库再搞muduo_base库