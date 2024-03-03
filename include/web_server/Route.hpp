// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include "controller/Test.hpp"

class Route{
    Route(){
        Test test(route);
    };
    ~Route() = default;
public:
    static auto &obj(){
        static Route alias;
        return alias;
    }
    
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &,uint32_t &)> route;
};