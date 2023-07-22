# web_server [原名称：bb HTTP server](https://github.com/135356/bb-HTTP-server)
跨平台网站服务器mvc框架
> 发布区 [Releases](https://github.com/135356/web_server/releases) 有已经编译好的二进制文件
### 视频演示(旧版本bb HTTP server3.0.1)
> [bilibili.com/video/BV1wS4y1t7gH](https://www.bilibili.com/video/BV1wS4y1t7gH?share_source=copy_web&vd_source=238df0657244861dbcd6cf34d4a3b4da)

### mode层 文件夹位置
> ./include/mode
### view层 文件夹位置
> ./build/html/
### controller层 文件夹位置
> ./include/controller  
> 需要将controller文件夹下的文件引入到 Route.hpp 然后在 Route.hpp 的构造函数里面申明

### 可执行文件位置：./build/web_server_run
> 运行项目./build/web_server_run start&  
> 停止项目./build/web_server_run stop  
> 默认端口为80，请确认端口没有被其它程序占用，项目运行后跟Apache一样，在浏览器地址栏输入 http://localhost/ 展示view层里面的内容

### 运行说明：
macOS系统，直接解压缩后通过终端运行：  
> ./build/web_server_run start

linux系统，解压缩后将./build/bb目录移动到/usr/local/bb：  
> mv ./build/bb /usr/local/bb  
> ./build/web_server_run start&