#ifndef HTML_TREE_H_INCLUDED
#define HTML_TREE_H_INCLUDED

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LEN 10000

typedef int status;

enum { START, END, NOEND, INNERTEXT, ATTRIBUTES };   //token的类型

//预处理的HTML链的结构
typedef struct token
{
	char *elem;
	int type;
	struct token *next;
}TOKEN;

//元素的属性结构
typedef struct attrvalue
{
	char *attrname;         //属性的名称
	char *value;            //属性的值
	struct attrvalue *next; //同元素下的另一个属性
}ATTRVALUE;

//元素结构
typedef struct elemtype
{
	char *elemname;         //元素的名称
	char *innertext;        //元素的中间文本
	struct elemtype *parallelize_elem;   //平行的元素指针
	struct elemtype *child_elem;         //孩子元素
	ATTRVALUE *sttributes;  //元素的属性
}ELEMTYPE;

char *HTML_filename = "HTML.txt";   //存放HTML程序的文档
int flag_wbr = 0;					//用于标记wbr是否配对

status Creat_HTML_List(void);
void test(void);
void (*token_handle)(char *token, int *token_len, int *sign);
void start_handle(char *token, int *token_len, int *sign);
void end_handle(char *token, int *token_len, int *sign);
void innertext_handle(char *token, int *token_len, int *sign);

#endif // HTML_TREE_H_INCLUDED
