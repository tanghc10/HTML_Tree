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

enum {START, END, NOEND, INNERTEXT, VALUE};   //token的类型

/**
 *预处理链表结点
 *elem存储信息
 *type存储结点类型
 *next指向下一个结点
**/
typedef struct token
{
	char *elem;
	int type;
	struct token *next;
}TOKEN;

/**
 *标签结点
 *classify存储标签分类
 *label_name存储标签名称
 *info存储与该节点直接相邻的网页信息
 *Para_label指向下一条同级标签
 *Sub_label指向下一条下级标签
 *Parent_label指向上一级标签
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

void Show_menu();
status Creat_HTML_List(void);
status Creat_HTML_Tree(Label_node **Root);
//void test(void);
status Free_HTML_Tree(Label_node **Root);
status Free_HTML_List(TOKEN *head);
int search_KeyWord(Label_node *T, char *Key, int *is_find);
void search_Title(Label_node *T, char *Key, int *is_find);
void Traverse_Title(Label_node *T);
void token_handle(char *token, int token_len, int sign);

#endif // HTML_TREE_H_INCLUDED
