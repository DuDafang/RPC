#include "mprpccontroller.h"

MprpcController::MprpcController()//构造函数初始化
{
    m_failed = false;
    m_errText = "";
}
void MprpcController::Reset() //重置
{
    m_failed = false;
    m_errText = "";
}
bool MprpcController::Failed() const//判断当前调用成功与否
{
    return m_failed;
}

std::string MprpcController::ErrorText() const
{
    return m_errText;
}

void MprpcController::SetFailed(const std::string& reason)
{
    m_failed = true;
    m_errText = reason;
}
//目前未实现具体的功能，直接写成空函数
void MprpcController::StartCancel(){}
bool MprpcController::IsCanceled() const {return false;} 
void MprpcController::NotifyOnCancel(google::protobuf::Closure* callback) {}