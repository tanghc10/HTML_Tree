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

enum { START, END, NOEND, INNERTEXT, ATTRIBUTES };   //token������

//Ԥ�����HTML���Ľṹ
typedef struct token
{
	char *elem;
	int type;
	struct token *next;
}TOKEN;

/**
 *��ǩ���
 *label_name�洢��ǩ����
 *Para_labelָ����һ��ͬ����ǩ
 *Sub_labelָ����һ���¼���ǩ
 *Label_infoָ��Label�д洢����Ϣ
 *Label_valueָ��Label�ĵ�һ��������Ϣ
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

int flag_wbr = 0;					//���ڱ��wbr�Ƿ����

status Creat_HTML_List(void);
status Creat_HTML_Tree(Label_node **Root);
void test(void);
void token_handle(char *token, int token_len, int sign);

#endif // HTML_TREE_H_INCLUDED
