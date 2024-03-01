// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include "bb/secure/Token.h"
#include "mode/dbA1_test.hpp" //调用mode

struct Test{
    Test(std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &,uint32_t &)> &route){
        //接口为：http://网站/test
        route["/test"] = [](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size,uint32_t &client_ip) {
            //操作数据库（更多内容在 https://github.com/135356/mysqlorm ）
            std::string data{};
            dbA1_test::obj().insertF("'小明',20,1"); //插入数据(注意顺序，字段允许留空如: dbA1_test::obj().insertF("'小明',,1") )
            dbA1_test::obj().where("age=20")->get(data); //获取age=20的那条数据
            //s_data为返回给客户端的字符串数据
            s_data.resize(data.size()+1024);
            //下面的内容，客户端将收到：{"state":"0","msg":"登出成功","data":"******"}
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","data":"%s"})", 0, "成功",data.c_str());
            s_data.resize(s_size);
        };
    }
};