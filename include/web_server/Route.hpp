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
    std::map<std::string,std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &)>> route;
    static auto &obj(){
        static Route alias;
        return alias;
    }
};