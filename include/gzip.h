// 2024 LongBang
#pragma once
#include <string>
#include <cstring>
#include <dirent.h>
#include "ccore/safe/Log.h"
#include "ccore/net/http/HttpConfig.h"
#include "zlib.h"

class gzip{
public:
    //压缩文件
    static int end(const char *file_name);
    //压缩html文件
    static int endHtml(const std::string &file_dir);
};