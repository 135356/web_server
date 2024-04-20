// 2024 LongBang
#pragma once
#include <map>
#include <string>
#include <sys/wait.h>
#include "ccore/Time.h"
#include "ccore/net/IpTcp.h"
#include "ccore/net/http/HttpConfig.h"
#include "ccore/net/http/Serve.h"
#include "Route.hpp"
#include "gzip.h"

class Work{
    Work(){
        signal(SIGABRT,stopF_); //当该进程收到停止信号时触发stopF_函数
        //压缩html文件
        if(cc::net::http::HttpConfig::obj().IS_GZIP_ == 1){
            gzip::endHtml("./"+cc::net::http::HttpConfig::obj().WEB_DIR_);
        }
    }
    ~Work()=default;
    static void stopF_(int signum){
        if(signum == SIGABRT){ //判断接收到的信号
            exit(0);
        }
    }
public:
    void singleF(){
        cc::net::http::Serve http(Route::obj().route);
    }
    void formalF(){
        while(true){
            pid_t pid = fork();
            if(pid == 0){ //等于0表示子进程提供服务
                singleF();
            }else{
                wait(0); //等待
            }
            cc::safe::Log::obj().info("进程退出");
        }
    }
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
            cc::safe::Log::obj().error("stop停止进程的时候出现错误");
        }
    }
    static void run(int &argc, char *argv[]){
        static Work work;
        if(argc > 1){
            if(strncmp(argv[1],"stop",4) == 0){
                work.stopF(argv[0]);
            }else if(strncmp(argv[1],"test",4) == 0){
                work.singleF();
            }else if(strncmp(argv[1],"start",3) == 0){
                work.formalF();
            }
        }else{
            work.singleF();
        }
    }
};