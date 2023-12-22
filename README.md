# web_server [原名称：bb HTTP server](https://github.com/135356/bb-HTTP-server)
跨平台网站服务器mvc框架
> 发布区 [Releases](https://github.com/135356/web_server/releases) 有已经编译好的二进制文件

### 依赖关系
> [135356/bb](https://github.com/135356/bb) 包含线程池、tcp长链接、http协议解析、文件管理、日志系统、字符加解密  
> [135356/mysqlorm](https://github.com/135356/mysqlorm) mysql映射

### mode层 文件夹位置
> ./include/mode
### view层 文件夹位置
> ./build/html/
### controller层 文件夹位置
> ./include/controller  
>> 需要将 controller 文件夹下的文件引入到 Route.hpp，然后在 Route.hpp 的构造函数里面实例化controller对象

创建 controller 示例（./controller/Test.hpp）:
```C++
struct Test{
    //在构造函数里面创建浏览路径
    Test(std::map<std::string, void (*)(unsigned &,std::map<std::string,std::string> &,std::string &,size_t &)> &route){
        //该路径的访问方式：http://网站/testURL
        route["/testURL"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            //s_data是要返回给客户端的字符串数据，1024是预设字符串长度
            s_data.resize(1024);
            //这里的结构类似于 s_data = "aabbcc字符串"
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登出成功");
            s_data.resize(s_size);
        };
    }
};
```
然后在 Route 构造函数里面完成实例化，示例（include/Route.hpp）:
```C++
#include "controller/test.hpp"
class Route{
    Route(){
        //在Route的构造函数里面实例化后，控制器就生效了
        test aaa(route);
    };
};
```

### 可执行文件位置：./build/web_server_run
> 运行项目./build/web_server_run start&  
> 停止项目./build/web_server_run stop  
> 默认端口为80，请确认端口没有被其它程序占用，项目运行后跟Apache一样，在浏览器地址栏输入 http://localhost/ 展示view层里面的内容

### 运行说明：
* macOS系统：
        直接解压缩即可
    > 示例：
        ./build/web_server_run start  //在 web_server的build 目录下运行项目  

* linux系统：
        解压缩后得到bb.tar与web_server.tar，将bb.tar继续解压缩后移动到/usr/local/bb目录，将web_server.tar继续解压后在 web_server/build 目录下运行项目   
    > 示例：
        mv ./bb /usr/local/bb   //将 bb 移动 /usr/local/bb  
        ./build/web_server_run start&   //在 web_server的build 目录下运行项目  