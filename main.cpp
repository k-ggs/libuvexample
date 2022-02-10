
#include<stdlib.h>
#include<iostream>

#include"uv.h"
static uv_loop_t* event_loop = NULL;
static uv_fs_t req;
static uv_fs_t write_req;
static uv_file fs_handle;

char mem_buffer[1024];

/*
uv_fs_t
result:每次请求的结果都是返回这个值;
打开文件：result返回打开文件句柄对象uv_file;
读文件：  result返回读到的数据长度;
写文件：  result返回写入的数据长度;
*/
static void uv_fs_read_cb(uv_fs_t* req)
{
 //打印读到的字节数
 printf("read %d byte\n", req->result);
 mem_buffer[req->result] = 0; //字符串结尾符;
 //打印数据
 printf("buffer:%s\n",mem_buffer);

 uv_fs_close(event_loop, req, fs_handle, NULL);
 uv_fs_req_cleanup(req);
}

static void uv_fs_open_cb(uv_fs_t* req)
{
 fs_handle = req->result;
 uv_fs_req_cleanup(req);
 printf("open success!\n");

 //读文件
 uv_buf_t buf = uv_buf_init(mem_buffer, 1024);
 uv_fs_read(event_loop, req, fs_handle, &buf, 1, 0, uv_fs_read_cb);
}


int main(int arg,char** args){
event_loop = uv_default_loop();
 //请求打开文件;
 uv_fs_open(event_loop, &req, "test.txt",0, _O_WRONLY, uv_fs_open_cb);
 printf("open success2!\n");
 //请求写文件
 uv_buf_t write_buf = uv_buf_init("helloworld!!!\n", 14);
 uv_fs_write(event_loop, &write_req, (uv_file)1, &write_buf, 1, 0, NULL);
 //释放请求req所占的资源;
 //如果请求函数不需要回调函数则下面可直接cleanup,否则把cleanup放到回调函数中执行;
 uv_fs_req_cleanup(&write_req);

 uv_run(event_loop, UV_RUN_DEFAULT);
 system("pause");

 return 0;

}