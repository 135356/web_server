# web_server [原名称：bb HTTP server](https://github.com/135356/bb-HTTP-server)
跨平台网站服务器mvc框架
> 发布区 [Releases](https://github.com/135356/web_server/releases)

### 依赖关系
> [135356/bb](https://github.com/135356/bb) http协议  
> [135356/mysqlorm](https://github.com/135356/mysqlorm) mysql映射  
> [gzip](https://mp.csdn.net/mp_blog/creation/editor/135407605) 文件压缩

### mode层 文件夹的位置
> include/mode  
> 示例文件: include/mode/dbA1_test.hpp  
### view层 文件夹的位置
> build/html/h5文件
### controller层 文件夹的位置
> include/controller  
> 示例文件: include/controller/Test.hpp  
### 路由文件
> include/web_server/Route.hpp  
> 在 Route 构造函数里面对controller实例化，如: Test test(route);

### 运行说明：
> 默认端口为80，请确认端口没有被其它程序占用，项目运行后跟Apache一样，在浏览器地址栏输入 http://127.0.0.1 即可展示view层的h5内容
* linux系统：  
    先把依赖程序解压安装到/usr/local/路径下，依赖项可在 [Releases](https://github.com/135356/web_server/releases)下载，分别为bb.tar 与 gzip.tar 将它们解压缩即可，如：/usr/local/bb 与 /usr/local/gzip  
    克隆项目源码到本地，在项目路径下创建build目录，将需要展示的h5文件放到build/html目录下，进入 build目录 然后执行 cmake 操作，如:  
    git clone https://github.com/135356/web_server.git  
    cd web_server  
    mkdir build  
    cd build  
    cmake ..  
    make  
    编译成功将得到 web_server_run 可执行文件  
    运行: ./web_server_run start&  
    停止: ./web_server_run stop