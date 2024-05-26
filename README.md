# web_server
Web服务器框架，大小仅8m，它不依赖apache与nginx，搭建非常简单，直接解压缩就可以搭建c++网站了。如果要求不高你甚至不需要了解c++就可以完成一个简单的web服务器（关键开发者就在你身边，所有功能都是我弄的，有啥问题联系我邮箱 a13535@foxmail.com）

> 视频  [部署演示](https://www.bilibili.com/video/BV1RM4m167gK/)  
> 发布区 [Releases](https://github.com/135356/web_server/releases)

### 依赖mysqlorm
> https://github.com/135356/mysqlorm 它负责链接mysql，有关数据库的操作都在 [135356/mysqlorm](https://github.com/135356/mysqlorm) 这个项目  

### 基本信息配置文件
````c++
文件路径：./build/13535_webserve.conf
配置信息：
    "IS_GZIP": 1, 是否开启gzip，1为开启0为关闭，开启后会自动将h5文件压缩为gzip格式的文件
    "IS_COROUTINE": 0, 是否开启协程，1为开启0为关闭，开启后会默认启用10个线程若干个协程，然后平均分配任务
    "IS_MTB_FILE": 1, 是否开启更多线程来处理大文件，1为开启0为关闭，根据负载分配更多的线程处理超过10m的文件
    "WEB_DIR": "html", h5文件放哪个位置，比如html就是项目build路径下的html
    "PORT": 80, 端口号，比如改成了8888，访问链接的时候就需要带上这个端口号，http://localhost:8888
    "ORIGIN": "*" 白名单，*表示所有链接，如果是多个链接中间用分号;隔开即可（不要有任何不相关的符号包括空格）
````

### 日志文件
> *./build/13535.log*
### mode层
> 文件夹的路径：include/mode  
> 示例文件: include/mode/dbA1_test.hpp  
> 负责与数据库交互，比如你可以通过dbA1_test::obj()链接到db_a1数据库的test数据表，通过dbA1_test::obj().insertF("'小明',20,1")这样的方式，你就可以向db_a1数据库的test数据表插入一条数据，有关于mode层的详细介绍在[135356/mysqlorm](https://github.com/135356/mysqlorm)  
### view层
> 文件夹的路径：build/html  
> 你可以将自己的h5文件放在build/html目录下，h5文件可以通过vue之类的框架创建，也可以直接写一个index.html文件放里面  
### Route文件
> 文件路径：include/web_server/Route.hpp  
> 你可以直接将api写在这个文件里面，但并不建议这样做，推荐你将api尽量细分后写到 controller层 里面  
### controller层
> 文件夹的路径：include/controller  
> 示例文件: include/controller/Test.hpp  
> 负责与客户端交互，比如你可以创建一个post链接配合mode层实现用户注册之类的操作  
> 需在 Route文件 里面，引入 controller层 的文件，并在 Route文件 的构造函数里面对 controller层 文件进行实例化（并不复杂，你打开 Route文件 看一下就知道了）  

### 运行说明：
> 默认端口为80，请确认端口没有被其它程序占用，项目运行后跟Apache一样，在浏览器地址栏输入 http://127.0.0.1 即可
* linux系统部署：  
    先下载依赖的库解压到 /usr/local/ 路径下。依赖的库可在[Releases](https://github.com/135356/web_server/releases)下载  
    再下载项目的源码解压到任意位置都可以，解压缩之后进入到这个项目，创建一个用于存放二进制文件的目录，比如build，然后进入到这个build目录，使用cmake进行编译即可。  

    示例：  
    >
        git clone https://github.com/135356/web_server.git  
        cd web_server  
        mkdir build  
        cd build  
        cmake ..  
        make   
        运行: ./web_server_run start&  
        停止: ./web_server_run stop
* 其它系统部署：暂不支持(懒得用其他平台去编译，关键部分功能比如epoll是不通用的，改起来麻烦)