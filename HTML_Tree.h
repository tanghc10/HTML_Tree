#ifndef HTML_TREE_H_INCLUDED
#define HTML_TREE_H_INCLUDED

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LEN 10000
#define MAX_LabelName_Length 50
typedef int status;

enum { START, END, NOEND, INNERTEXT, ATTRIBUTES };   //token的类型

//预处理的HTML链的结构
typedef struct token
{
	char *elem;
	int type;
	struct token *next;
}TOKEN;

/**
 *标签结点
 *label_name存储标签名称
 *Para_label指向下一条同级标签
 *Sub_label指向下一条下级标签
 *Label_info指向Label中存储的信息
 *Label_value指向Label的第一条属性信息
**/
typedef struct label_node{
	int classify;
	char label_name[MAX_LabelName_Length];
	char *info;
	struct label_node *Para_label;
	struct label_node *Sub_label;
	struct label_node *Parent_label;
	struct label_node *Label_value;
}Label_node;

int flag_wbr = 0;					//用于标记wbr是否配对

status Creat_HTML_List(void);
status Creat_HTML_Tree(Label_node **Root);
void test(void);
void token_handle(char *token, int token_len, int sign);

#endif // HTML_TREE_H_INCLUDED
