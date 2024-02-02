// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include "controller/User.hpp"

class Route{
public:
    static Route &obj(){
        static Route alias;
        return alias;
    }
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &,uint32_t &)> route = {
        //测试
        {"/test",      [](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size,uint32_t &client_ip) {
            s_data.resize(1024);
            for(auto &v:r_data){
                printf("%s:%s\n",v.first.c_str(),v.second.c_str());
            }
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "成功");
            s_data.resize(s_size);
        }}
    };
private:
    Route(){
        User user(route);
    };
    ~Route() = default;
};