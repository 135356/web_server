// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include "ccore/net/http/Controller.hpp"
#include "mode/dbA1_test.hpp" //调用dbA1_test数据库mode

struct Test:public cc::net::http::Controller{
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    Test(std::map<std::string,std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &)>> &route){
        route["get"] = {
            {
                //接口为：http://网站/test
                "/test",[](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
                    s_data.resize(1024);
                    for(auto &v:r_data){
                        printf("%s:%s\n",v.first.c_str(),v.second.c_str());
                    }
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "post成功");
                    s_data.resize(s_size);
                },
            }
        };
        route["post"] = {
            {
                //测试数据库
                "/test_db",[](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
                    std::string data{};
                    dbA1_test::obj().insertF("'小明',20,1"); //插入数据(注意顺序，字段允许留空如: dbA1_test::obj().insertF("'小明',,1") )
                    dbA1_test::obj().where("age=20")->get(data); //获取age=20的那条数据
                    //s_data为返回给客户端的字符串数据
                    s_data.resize(data.size()+1024);
                    //下面的内容，客户端将收到：{"state":"0","msg":"登出成功","data":"******"}
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","data":"%s"})", 0, "get成功",data.c_str());
                    s_data.resize(s_size);
                }
            }
        };
    }
};