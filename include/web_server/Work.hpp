// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include <sys/wait.h>
#include "bb/Time.h"
#include "bb/net/IpTcp.h"
#include "bb/net/http/HttpConfig.hpp"
#include "bb/net/http/Serve.h"
#include "Route.hpp"
#include "gzip.h"

class Work{
    Work(){
        signal(SIGABRT,stopF_); //当该进程收到停止信号时触发stopF_函数
        //压缩html文件
        if(bb::net::http::HttpConfig::obj().gzip){
            gzip::endHtml("./"+bb::net::http::HttpConfig::obj().web_dir);
        }
    }
    ~Work()=default;
    static void stopF_(int signum){
        if(signum == SIGABRT){ //判断接收到的信号
            exit(0);
        }
    }
public:
    //测试模式
    void testF(){
        bb::net::http::Serve sevre(Route::obj().route);
    }
    //正式模式
    void formalF(){
        while(true){
            pid_t pid = fork();
            if(pid == 0){ //等于0表示子进程提供服务
                bb::net::http::Serve sevre(Route::obj().route);
            }else{
                wait(0); //等待
            }
            bb::secure::Log::obj().info("进程退出");
        }
    }
    //停止进程
    void stopF(char path[]){
        size_t start_i,end_i=strlen(path);
        for(start_i=end_i;start_i>0;start_i--){
            if(path[start_i] == '/'){
                start_i++;break;
            }
        }
        char name[end_i-start_i];
        memmove(&name[0],&path[start_i],end_i-start_i);

        char cmd[end_i-start_i+11];
        sprintf(cmd,"killall -%d %s",SIGABRT,name);
        cmd[end_i-start_i+11]='\0';

        if(system(cmd) == -1){
            bb::secure::Log::obj().error("stop停止进程的时候出现错误");
        }
    }
    static auto &obj(){
        static Work alias;
        return alias;
    }
};