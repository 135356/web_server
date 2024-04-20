#include "gzip.h"

int gzip::end(const char *file_name){
    //判断压缩文件是否已经存在
    char gz_name[64];
    snprintf(gz_name, 64, "%s.gz", file_name);
    if (access(gz_name, F_OK) == 0) {
        return 1;
    }

    //打开要压缩的源文件
    FILE *in_fp = fopen(file_name, "rb");
    if (in_fp == NULL) {
        cc::safe::Log::obj().warn("gzip无法打开源文件");
        return -1;
    }
    
    //创建并打开目标压缩文件
    gzFile out_fp = gzopen(gz_name, "wb");
    if (out_fp == NULL) {
        cc::safe::Log::obj().warn("gzip无法创建或打开压缩文件");
        return -2;
    }

    //将读取到的数据写入压缩文件
    char buf[4096];
    int read_size{};
    while((read_size = fread(buf, 1, 4096, in_fp)) > 0) {
        gzwrite(out_fp, buf, read_size);
    }

    fclose(in_fp); //关闭源文件
    gzclose(out_fp); //关闭压缩文件
    return 0;
}

int gzip::endHtml(const std::string &file_dir){
    //打开目录
    DIR *dir_p = opendir(file_dir.c_str());
    if(dir_p == NULL){
        cc::safe::Log::obj().warn(file_dir+"不存在");
        return -1;
    }

    //循环读取目录中的文件和子目录进入点
    struct dirent *f_info;
    while((f_info = readdir(dir_p)) != NULL){
        //忽略当前目录与上级目录
        if(strcmp(f_info->d_name,".") == 0 || strcmp(f_info->d_name,"..") == 0){
            continue;
        }
        std::string file_path = file_dir +'/'+ f_info->d_name;
        switch(f_info->d_type){
            case DT_DIR: //目录
                endHtml(file_path);
            break;
            case DT_REG: //常规文件
                int name_index = file_path.rfind(".");
                std::string file_suffix = file_path.substr(name_index+1);
                //如果已经存在gz文件，表示已经执行过压缩了
                if(cc::net::http::HttpConfig::obj().content_type_[file_suffix].is_gzip){
                    end(file_path.c_str());
                }
            break;
        }
    }
    //关闭目录
    closedir(dir_p);
    return 0;
}