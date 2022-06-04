#pragma once

#define MAXSIZE 4096

//不指定编码，使用时可能导致乱码
//#pragma execution_character_set("utf-8")  

//id指对应网站的端口号
int hk_404(char *id, char *response) ;

//恢复网站
int hk_restore(char *id, char *response);

//网页篡改攻击，para表示填充内容
int hk_tamper(char *id, char *para, char *response);

//查看攻击记录
int hk_record(char *id, char *response);