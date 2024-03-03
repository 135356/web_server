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
    void singleF(){
        bb::net::http::Recv f([](bb::net::http::Serve *http,bb::net::http::Analyze *info,bb::net::IpTcpConfig::ClientStruct *client_struct)->bool{
            std::string s_data{}; //要发送的内容
            size_t s_size{}; //要发送的长度
            printf("aaa:%s\n",info->r_buf);
            if (Route::obj().route.count(info->path)) {
                if(info->method == "POST"){
                    if(info->head_map["content-type"] == "application/json"){
                        std::map<std::string, std::string> r_data;
                        if(info->getPost(r_data)){ //获取post数据
                            Route::obj().route[info->path](r_data,s_data, s_size,client_struct->ip);
                            return http->sendHeadF(client_struct->fd,info,"post","html","200 OK",&s_data[0],s_size,false,false);
                        }else{
                            return http->sendHeadF(client_struct->fd,info,"get","html","400 post数据获取失败",nullptr,0,false,false);
                        }
                    }else{
                        return http->sendHeadF(client_struct->fd,info,"get","html","400 (content-length is application/json) or (content-length error)",nullptr,0,false,false);
                    }
                }else{
                    Route::obj().route[info->path](info->get_map,s_data,s_size,client_struct->ip);
                    return http->sendHeadF(client_struct->fd,info,info->method,"html","200 OK",&s_data[0],s_size,false,false);
                }
            }else{
                if (info->method == "GET"){
                    return http->sendFileF(client_struct->fd,info); //网页 或 文件请求
                }else if (info->method == "POST") {
                    return http->sendHeadF(client_struct->fd,info,"get","html","404 Not POST route",nullptr,0,false,false);
                } else if (info->method == "OPTIONS") {
                    return http->sendHeadF(client_struct->fd,info,"options","html","200 ok",nullptr,0,false,false);
                }else{
                    return http->sendHeadF(client_struct->fd,info,"get","html",("400 Not Supported: "+info->method),nullptr,0,false,false);
                }
            }
        });
    }
    void formalF(){
        while(true){
            pid_t pid = fork();
            if(pid == 0){ //等于0表示子进程提供服务
                singleF();
            }else{
                wait(0); //等待
            }
            bb::secure::Log::obj().info("进程退出");
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
            bb::secure::Log::obj().error("stop停止进程的时候出现错误");
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