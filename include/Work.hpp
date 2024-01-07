#pragma once
#include <map>
#include <string>
#include <sys/wait.h>
#include "bb/Time.h"
#include "bb/net/http/Config.hpp"
#include "bb/net/http/Serve.h"
#include "Route.hpp"
#include "gzip.h"

class Work{
    Work(){
        signal(SIGABRT,stopF_); //当该进程收到停止信号时触发stopF_函数
        //压缩html文件
        if(bb::net::http::Config::obj().gzip){
            gzip::endHtml("."+bb::net::http::Config::obj().web_dir);
        }
    }
    ~Work()=default;
    //监听80端口
    void runF_(int port=bb::net::http::Config::obj().serve_port){
        bb::net::TcpEpoll epoll(port,6);
        epoll.runF([](int &client_fd,uint32_t &client_ip)->bool{
            auto routeF = [](uint32_t &client_ip,bb::net::http::Serve *http_serve)->bool{
                std::string s_data{}; //要发送的内容
                size_t s_size{}; //要发送的长度
                if (http_serve->info->method == "GET"){
                    if (Route::obj().route.count(http_serve->info->path)) {
                        //运行路由里面的方法
                        Route::obj().route[http_serve->info->path](client_ip,http_serve->info->get_map,s_data,s_size);
                        return http_serve->sendHeadF("get",bb::net::http::Config::obj().content_type_["html"].alias,&s_data[0],s_size,"200 OK",false,false);
                    }else{
                        //网页 或 其它请求
                        return http_serve->sendHtmlF();
                    }
                }else if (http_serve->info->method == "POST") {
                    if (Route::obj().route.count(http_serve->info->path)) {
                        if(http_serve->info->head_map["content-type"] == "application/json"){
                            std::map<std::string, std::string> r_data;
                            if(http_serve->recvJsonF(r_data)){ //获取post数据
                                Route::obj().route[http_serve->info->path](client_ip,r_data,s_data, s_size);
                                return http_serve->sendHeadF("post",bb::net::http::Config::obj().content_type_["html"].alias,&s_data[0],s_size,"200 OK",false,false);
                            }
                        }
                        return http_serve->sendHeadF("get",bb::net::http::Config::obj().content_type_["html"].alias,nullptr,0,"400 (content-length is application/json) or (content-length error)",false,false);
                    }else{
                        return http_serve->sendHeadF("get",bb::net::http::Config::obj().content_type_["html"].alias,nullptr,0,"404 Not POST route",false,false);
                    }
                } else if (http_serve->info->method == "OPTIONS") {
                    return http_serve->sendHeadF("options",bb::net::http::Config::obj().content_type_["html"].alias,nullptr,0,"200 ok",false,false);
                }else{
                    return http_serve->sendHeadF("get",bb::net::http::Config::obj().content_type_["html"].alias,nullptr,0,("400 Not Supported: "+http_serve->info->method),false,false);
                }
            };

            bb::net::http::Serve *http_serve = new bb::net::http::Serve(client_fd);
            if(!http_serve->info){return false;} //如果http没有解析出有用数据就直接返回
            
            //printf("r_buf:%s\n",http_serve->r_buf);

            bool is = routeF(client_ip,http_serve);
            delete http_serve;
            return is;
        });
    };
    static void stopF_(int signum){
        if(signum == SIGABRT){ //判断接收到的信号
            exit(0);
        }
    }
public:
    //测试模式
    void testF(){
        runF_();
    }
    //正式模式
    void formalF(){
        while(true){
            pid_t pid = fork();
            if(pid == 0){ //等于0表示子进程提供服务
                runF_();
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
    static Work &obj(){
        static Work obj;
        return obj;
    }
};