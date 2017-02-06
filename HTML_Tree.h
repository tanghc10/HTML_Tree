#ifndef HTML_TREE_H_INCLUDED
#define HTML_TREE_H_INCLUDED

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_LEN 10000

typedef int status;

enum { START, END, NOEND, INNERTEXT, ATTRIBUTES };   //token������

//Ԥ�����HTML���Ľṹ
typedef struct token
{
	char *elem;
	int type;
	struct token *next;
}TOKEN;

//Ԫ�ص����Խṹ
typedef struct attrvalue
{
	char *attrname;         //���Ե�����
	char *value;            //���Ե�ֵ
	struct attrvalue *next; //ͬԪ���µ���һ������
}ATTRVALUE;

//Ԫ�ؽṹ
typedef struct elemtype
{
	char *elemname;         //Ԫ�ص�����
	char *innertext;        //Ԫ�ص��м��ı�
	struct elemtype *parallelize_elem;   //ƽ�е�Ԫ��ָ��
	struct elemtype *child_elem;         //����Ԫ��
	ATTRVALUE *sttributes;  //Ԫ�ص�����
}ELEMTYPE;

char *HTML_filename = "HTML.txt";   //���HTML������ĵ�
int flag_wbr = 0;					//���ڱ��wbr�Ƿ����

status Creat_HTML_List(void);
void test(void);
void (*token_handle)(char *token, int *token_len, int *sign);
void start_handle(char *token, int *token_len, int *sign);
void end_handle(char *token, int *token_len, int *sign);
void innertext_handle(char *token, int *token_len, int *sign);

#endif // HTML_TREE_H_INCLUDED
