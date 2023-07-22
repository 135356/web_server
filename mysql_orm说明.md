# MysqlOrm
###### Mysql数据库映射，请先正确安装并运行 **mysql-8** 之后在运行该项目
> 请到发布区 Releases 下载已经编译好的内容。如遇问题欢迎骚扰: x135356@sina.com
### 视频演示(1.0分支)
> [bilibili.com/video/BV1ey4y1L7UR/](https://www.bilibili.com/video/BV1ey4y1L7UR/)
### 数据库基本信息配置文件:
> *./build/bb_mysql_orm_config.conf*
### 日志文件:
> *./build/bb.log*

### 直接执行sql语句（不推荐）
```c++
int query_(const std::string &sql)
    参数：sql 语句如:show databases;
    返回值：成功返回0，失败返回-1
```
### 判断数据库是否存在
```c++
int isDB(const std::string &name);
    参数：name 数据库的名称
    返回值：发生错误返回-1，不存在返回0，存在返回1
```
### 判断数据表是否存在
```c++
int isTable(const std::string &name);
    参数：name 数据表的名称
    返回值：发生错误返回-1，不存在返回0，存在返回1
```
### 切换数据库(在对数据库操作之前用该方法切换)
```c++
int useDB(const std::string &name)
    参数：name 数据库名称
    返回值：成功返回0，失败返回-1
```
### 切换数据表(在对数据表操作之前用该方法切换)
````c++
int useTable(const std::string &name)
    参数：name 数据表名称
    返回值：成功返回0，失败返回-1
````
### 创建数据库
````c++
int createDB(const std::string &name);
    参数：name 数据库名称
    返回值：成功返回0，失败返回-1
````
### 改数据库名称
````c++
int upDB(const std::string &old_name, const std::string &new_name)
    参数：
        old_name 原来的数据库名称
        new_name 新的数据库名称
    返回值：成功返回0，失败返回-1
````
### 删除数据库
````c++
int delDB(const std::string &name)
    参数：name 数据库名称
    返回值：成功返回0，失败返回-1
````

### mode层
````c++
首先创建一个mode文件如：./mode/abC1_aaaBbb.hpp
名称定义: 数据库_数据表  
> 大小写敏感，如: abC1_aaaBbb结果为:【数据库：ab_c1 与 数据表：aaa_bbb】  

````


### 创建数据表
````c++
int createTable(const std::string &name, void (*createF)(dml *));
    参数：
        name 数据表名称
        createF 设定字段名与字段类型的函数
            tinyint_("name") 短短短整型
            tinyint_("name",4) 同上，指定长度为4
            -
            smallint_("name") 短短整型
            -
            mediumint_("name") 短整型
            -
            int_("name") 整型
            -
            bigint_("name") 长整型
            --
            float_("name") 浮点型,左侧最大值21位，右侧最大值占4位，右侧超出4位会被截断
            float_("name",255); 同上，指定长度
            float_("name",255,30); 同上，指定精度
            -
            double_("name") 双精度型
            -
            decimal_("name") 数字型,不存在精度损失
            ---
            string_("name") 字符串型
            string_("name",1000) 同上，指定长度
            -
            text_("name") 文本类型
            ---
            date_("name") 时间类型
            dateAt_("name") 会增加两个字段自动填充创建时间与修改时间，分别是created_at(创建时间)与updated_at(修改时间)
            --------
            unsigned_(); 无符号的
            unique_(); 唯一的
            nullable_(); 允许值为null
            default_("value"); 默认值
            zerofill_(); 填充0固定长度：123=0000000123
            comment_("words"); 注解
    返回值：成功返回0，失败返回-1
    示例:参考./mode/dbA1_test.hpp
````
### 改表名称
````c++
int upTable(const std::string &new_name)
    参数：name 新的数据表名称
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
````
### 删除数据表
````c++
int delTable()
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
````
### 清空数据表
````c++
int truncate()
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
````
### 追加列
````c++
int addCol(void (*createF)(dml *))
    参数:createF 设定字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
    示例:
        //追加a100字段，字段类型int，允许为null，注释内容为"这是a100"
        a.addCol([](auto *data){
            data->int_("a100")->nullable_()->comment_("这是a100");
        });
````
### 改列名称与类型
````c++
int upColName(const std::string &old_key, void (*createF)(dml *))
    参数:
        old_key 原来的字段名称
        createF 新的字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
    示例:
        //将a1字段改为a100，字段类型int，允许为null，注释内容为"这是a100"
        a.updateCol("a1",[](auto *data){
            data->int_("a100")->nullable_()->comment_("这是a100");
        });
````
### 改列类型
````c++
int upColType(void (*createF)(dml *))
    参数:createF 设定字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
    示例:
        //修改a1字段的类型为int，允许为null，注释内容为"这是a1"
        a.updateType([](auto *data){
            data->int_("a1")->nullable_()->comment_("这是a1");
        });
````
### 删除列
````c++
int delCol(const std::string &key)
    参数:key 要删除的字段名
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
//删除多个列
int delCols(const std::vector<std::string> &key_arr)
    参数:key_arr 支持同时删除多个字段如：["a1","a2"]
    在void update_()里面使用该方法
````
### 添加索引
````c++
int addIndex(const std::string &key, const short &type = 0)
    参数:
        key 将该字段设定为索引
        type 索引类型
            0 默认值，普通索引
            1 唯一索引
            2 全文索引(只支持 char、varchar和text)
            3 主键索引
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
````
### 删除索引
````c++
int delIndex(const std::string &key)
    参数: key 要被删除索引字段的名称
    返回值：成功返回0，失败返回-1
    在void update_()里面使用该方法
````

### 插入数据
````c++
int insert(const std::vector<std::array<std::string, 2>> &data)
    参数: data 要被插入的数据(键值对)
    返回值：成功返回0，失败返回-1
    示例:
        //插入a1字段的值为1,a2字段的值为2,a3字段的值为3
        dbA1_test::obj().create({{"a1","1"},{"a2","2"},{"a3","3"}});
````
### 批量插入数据
````c++
int insert(const std::vector<std::string> &key,const std::vector<std::vector<std::string>> &value)
    参数:
        key 要插入数据的key数组
        data 要被插入的数据集合，每一段都要与key数组匹配
    返回值：成功返回0，失败返回-1
    示例:
        //将对a1、a2、a3字段进行插入，内容分别是1、2、3与21、22、23
        dbA1_test::obj().create({"a1","a2","a3"},{{"1","2","3"},{"21","22","23"}});
````
### 改一行数据
````c++
int update(const std::vector<std::array<std::string, 2>> &data)
    参数: data 要修改的数据 键值对
    返回值：成功返回0，失败返回-1
    示例:
        //查找a1=aa的那一条数据，内容修改为:a2=bb，a3=cc
        dbA1_test::obj().where("a1","aa")->update({{"a2","bb"},{"a3","cc"}});
````
### 删除行
````c++
int del()
    返回值：成功返回0，失败返回-1
    示例:
        //查找a1=aa的那一条数据 删除掉
        dbA1_test::obj().where("a1","aa")->del();
````
### 获取指定字段的内容
````c++
dql *select(const std::string &key = "*")
    参数: key 只获取该定字段的内容(默认值"*"表示所有字段的内容)
    返回值：指向自己的指针
    示例:
        //只要a1字段的内容，where条件为a1大于0的数据，show显示出来
        dbA1_test::obj().select("a1")->where("a1",">","0")->show();
dql *selectArr(const std::vector<std::string> &key)
    参数: key 只获取指定字段的内容(一组字段名称)
    返回值：指向自己的指针
    示例:
        //只要a1与a2字段的内容，where条件为a1大于0的数据，show显示出来
        dbA1_test::obj().selectArr({"a1", "a2"})->where("a1",">","0")->->show();
````
### 以id获取内容
````c++
dql *find(const unsigned long &id = 1)
    参数: id
    返回值：指向自己的指针
    示例:
        //寻找id等于1的数据，显示出来
        dbA1_test::obj().find(1)->show();
````
### where等于条件
````c++
dql *where(const std::string &key, const std::string &value)
dql *where(const std::string &key, const double &value)
    参数:
        key 字段名(字符串型)
        value 值(字符串 或 double数字类型)
    说明: 获取指定key 下的value 相等的一类数据
    返回值：指向自己的指针
    示例:
        //寻找a1等于aa的数据，进行删除
        dbA1_test::obj().where("a1","aa")->del();
        //寻找a1等于aa并且a2等于456的数据，进行查看
        dbA1_test::obj().where("a1","aa")->where("a2",456)->show();
````
### where其它条件
````c++
dql *where(const std::string &key, const std::string &symbols, const std::string &value)
dql *where(const std::string &key, const std::string &symbols, const double &value)
    参数:
        key 字段名(字符串型)
        symbols 支持的符号有: 大于(>)、小于(<)、等于(=)、大于等于(>=)、(<=)、(=)
        value 值(double数字类型)
    说明: 指定判断条件，如 id > 0 的一类数据
    返回值：指向自己的指针
    示例: 
        dbA1_test::obj().where("a1",">",100)->where("a2","<",999)->show();
````
### notWhere主要用于值为null的正确获取
````c++
dql *notWhere(const std::string &key, const std::string &value)
dql *notWhere(const std::string &key, const unsigned long &value)

dql *notWhere(const std::string &key, const std::string &symbols, const std::string &value)
dql *notWhere(const std::string &key, const std::string &symbols, const unsigned long &value)

dql *orWhere(const std::string &key, const std::string &value)
dql *orWhere(const std::string &key, const unsigned long &value)

dql *orWhere(const std::string &key, const std::string &symbols, const std::string &value)
dql *orWhere(const std::string &key, const std::string &symbols, const unsigned long &value)
````
### 排序方式
````c++
dql *order(const std::string &key, const std::string &type = "ASC")
    参数:
        key 字段名(以该字段进行排序 如:id)
        type 升序ASC，降序DESC
    返回值：指向自己的指针
````
### 区间查询，包含边界值
````c++
dql *between(const std::string &key, const unsigned long &start_value, const unsigned long &end_value)
    参数:
        key 字段名(以该字段进行查找)
        start_value 开始值
        end_value 结束值
    说明: 查询指定key下的内容，start_value到end_value之间的数据，包含边界值
    返回值：指向自己的指针
````
### 获取值为null的数据
````c++
dql *isNull(const std::string &key)
    参数:
        key 字段名(以该字段进行查找)
    说明: 查询指定key下的内容为null的数据
    返回值：指向自己的指针
````
### 获取值不为null的数据
````c++
dql *isNotNull(const std::string &key)
    参数:
        key 字段名(以该字段进行查找)
    说明: 查询指定key下的内容不为null的数据
    返回值：指向自己的指针
````
### like查找 %(匹配任意多个字符) _(匹配单一字符) \(转义)
````c++
dql *like(const std::string &key, const std::string &value)
````
### 查看获取到的数据
````c++
int show()
````
### 查看索引
````c++
int showIndex()
````
### 用于查看执行了多少次
````c++
int explain()
````
### 上一页
````c++
int limitLast(std::string &result,const unsigned &start_id,const unsigned &length = 100)
    参数:
        result 查询到的结果
        start_id 小于该id的数据
        length 每页显示多少条数据，默认100条
    返回值：失败返回-1，成功返回0
    示例：
        std::string str_data{};
        dbA1_test::obj().select("id")->limitLast(str_data,100);
        std::cout<<str_data<<std::endl;
````
### 下一页
````c++
int limitNext(std::string &result,const unsigned &end_id,const unsigned &length = 100)
    参数:
        result 查询到的结果
        end_id 大于该id的数据
        length 每页显示多少条数据，默认100条
    返回值：失败返回-1，成功返回0
    示例：
        std::string str_data{};
        dbA1_test::obj().select("id")->limitNext(str_data,1);
        std::cout<<str_data<<std::endl;
````
### 特定页
````c++
int limitSpecific(std::string &result,const unsigned &page = 1,const unsigned &length = 100)
    参数:
        result 查询到的结果
        page 要查看第几页
        length 每页显示多少条数据，默认100条
    返回值：失败返回-1，成功返回0
    示例：
        std::string str_data{};
        dbA1_test::obj().select("id")->limitSpecific(str_data,1);
        std::cout<<str_data<<std::endl;
````
### 获取数据
````c++
int get(std::string &result)
    参数:
        result 查询到的结果
    示例：
        std::string str_data{};
        bb_test::obj().where("id",1)->get(str_data);
        std::cout<<str_data<<std::endl;
````