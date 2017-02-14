#include"HTML_tree.h"

TOKEN *head_token = NULL, *tail_token = NULL;
Label_node *Root = NULL;

int main(void)
{
    Show_menu();
	return 0;
}

void Show_menu(){
    int command;
	char buffer[MAX_LEN];
	int is_find;
	printf("---------------------菜单------------------\n");
	printf("            1:创建HTML_DOM文档树\n");
	printf("            2:按照关键词查找文本信息\n");
	printf("            3:查找文章标题\n");
	printf("            4.销毁HTML_DOM文档树\n");
	printf("            0:退出系统\n");
	printf("----------------欢迎使用本系统-------------\n");
	while (scanf("%d", &command) == 1)
	{
		getchar();
		switch (command)
		{
		case 1:
			if (Creat_HTML_List() == TRUE && Creat_HTML_Tree(&Root) == TRUE)
                printf("HTML_DOM文档树已创建成功\n");
			break;
		case 2:
		    if (Root == NULL){
                printf("无可用HTML_DOM树！\n请创建后再进行操作.\n");
                break;
		    }
			printf("请输入需要查找的关键字\n");
			scanf("%s", buffer);
			getchar();
			is_find = 0;
			search_KeyWord(Root, buffer, &is_find);
			if (is_find == 0){
                printf("查找不到含有 %s 的文本！", buffer);
			}
			break;
		case 3:
		    if (Root == NULL){
                printf("无可用HTML_DOM树！\n请创建后再进行操作.\n");
                break;
		    }
			printf("请输入要查找的标题关键词\n");
			scanf("%s", buffer);
			getchar();
			is_find = 0;
			search_Title(Root, buffer, &is_find);
			if (is_find == 0){
                printf("查找不到含有 %s 的标题！\n\n 所有的标题如下：\n");
                Traverse_Title(Root);
			}
			break;
        case 4:
            if (Root == NULL){
                printf("HTML_DOM树不存在或已被销毁！\n请创建后再进行操作.\n");
                break;
		    }
            Free_HTML_Tree(Root);
            Root = NULL;
            printf("销毁成功！\n");
            break;
		case 0:
			printf("退出系统\n");
			return;
		default:
		    printf("输入错误，请重新输入：\n");
			continue;
		}
		getchar();
		system("cls");
		printf("---------------------菜单------------------\n");
        printf("            1:创建HTML_DOM文档树\n");
        printf("            2:按照关键词查找文本信息\n");
        printf("            3:查找文章标题\n");
        printf("            4.销毁HTML_DOM文档树\n");
        printf("            0:退出系统\n");
        printf("----------------欢迎使用本系统-------------\n");
	}
	return ;
}

status Creat_HTML_List(void){
	FILE *pfile = NULL;
	FILE *pfile1 = NULL;
	char ch;					//用于逐个读取文件中的字符
	char token[MAX_LEN] = "";   //用于暂时存放读取出来的token
	char buffer[MAX_LEN] = "";  //用于暂时存放读取出来的标签名称
	int IS_ignore = FALSE;		//用于标记注释是否忽略，默认为不可忽略
	int token_len = 0;			//用于标记token的长度
    int flag_wbr = 0;			//用于标记文字中的标签是否配对
	int sign = 0;
	int start = 0;
	int end = 0;

	head_token = (TOKEN *)malloc(sizeof(TOKEN));	//分配预处理链头节点
	head_token->next = NULL;
	TOKEN *p_token = head_token;		//用于存放预处理链头节点的指针

    char HTML_filename[100];   //存放HTML程序的文档
    printf("请输入要打开的文档:\n");
    scanf("%s", HTML_filename);
    getchar();
	if ((pfile = fopen(HTML_filename, "r")) == NULL)
	{
		printf("fail to open the file\n");
		return FALSE;
	}
	if ((pfile1 = fopen("output.txt", "w")) == NULL)
	{
		printf("fail to open the file\n");
		return FALSE;
	}

    Root = (Label_node*)malloc(sizeof(Label_node));
	while ((fscanf(pfile, "%c", &ch)) > 0)      //直到读到文本最后的'\0'为止
	{
		if(ch == '<')
		{
			fscanf(pfile, "%c", &ch);
			if(ch == '/')		//对于有直接以'/'结尾的token直接处理
			{
				fscanf(pfile, "%c", &ch);
				while (ch != '>')
				{
					token[token_len++] = ch;
					fscanf(pfile, "%c", &ch);
				}
				token[token_len] = '\0';
				if(!IS_ignore)
                {
                    fprintf(pfile1, "end  : %s\n", token);
                    sign = END;
                    token_handle(token, token_len, sign);
                }
				if(strstr(token, "style") != NULL || strstr(token, "script") != NULL || strstr(token, "noscript") != NULL || strstr(token, "textarea") != NULL)
					IS_ignore = FALSE;
				if(!IS_ignore)
                    end++;
				strcpy(token,"");
				token_len = 0;
			}
			else if(ch == '!')  //对注释的处理
            {
                fscanf(pfile, "%c", &ch);
                if(ch == '-')
                {
                    fscanf(pfile, "%c", &ch);   //取出注释!的--
                    fscanf(pfile, "%c", &ch);
                    while(TRUE)
                    {
                        while(ch != '-')
                            fscanf(pfile, "%c", &ch);
                        fscanf(pfile, "%c", &ch);
                        if(ch == '-'){
                            fscanf(pfile, "%c", &ch);
                            if (ch == '>')
                                break;
                        }

                    }
                }
                else
                {
                    while(ch != '>')
                        fscanf(pfile, "%c", &ch);
                }
            }
            else                //对开始标签的处理
			{
				while (ch != '>')
				{
					token[token_len++] = ch;
					fscanf(pfile, "%c", &ch);
				}
				token[token_len] = '\0';
                sscanf(token, "%[^ ]", buffer);
                if(!IS_ignore)
                    start++;
				if(strstr(buffer, "style") != NULL || strstr(buffer, "script") != NULL || strstr(token, "noscript") != NULL || strstr(buffer, "textarea") != NULL)
                    IS_ignore = TRUE;
                if(!IS_ignore)
                {
                    fprintf(pfile1, "start: %s\n", buffer);
                    //token_handle(buffer, strlen(buffer), START);
                    sign = START;
                }
                if(strcmp(buffer, "area") == 0 || strcmp(buffer, "base") == 0 || strcmp(buffer, "col") == 0 ||strcmp(buffer,"embed") == 0 ||
                   strcmp(buffer, "frame") == 0 || strcmp(buffer, "hr") == 0 ||strcmp(buffer, "img") == 0 || strcmp(buffer, "input") == 0 ||
                   strcmp(buffer, "keygen") == 0 || strcmp(buffer, "link") == 0 || strcmp(buffer, "param") == 0||
                   strcmp(buffer, "br") == 0 || strcmp(buffer, "meta") == 0 || strcmp(buffer, "basefont") == 0 || strcmp(buffer, "source") == 0)
                {
                    sign = NOEND;
                    start--;
                }
                else if(strcmp(buffer, "wbr") == 0)
                {
                    if(flag_wbr)
                    {
                        sign = END;
                        start--;
                        end++;
                        flag_wbr--;
                    }
                    else
                        flag_wbr++;
                }
                if(!IS_ignore)
                    token_handle(buffer, strlen(buffer), sign);
                char *p = NULL;
                char *href[100];
                if ((p = strstr(token, "href")) != NULL){
                    sscanf(p, "%*[^\"]\"%[^\"]", href);
                    if (strstr(href, "http") != NULL || strstr(href, "www") != NULL){
                       // printf("%s: %s\n", buffer, href);
                        sign = VALUE;
                        token_handle(href, strlen(href), sign);
                    }
                }
                strcpy(buffer, "");
				strcpy(token, "");
				token_len = 0;
			}
		}
		else
		{
			if(IS_ignore)
				continue;
			else if(ch == '\n' || ch == ' ' || ch == '\t')
				continue;
			else
			{
			    int flag = 0;
			    char *p;
				while (ch != '<')
				{
				    if(ch == '(')
                    {
                        fscanf(pfile, "%c", &ch);
                        while(ch != ')')
                            fscanf(pfile, "%c", &ch);
                        fscanf(pfile, "%c", &ch);
                        continue;
                    }
					token[token_len++] = ch;
					if (ch == '&')
                        flag = 1;
                    if (flag){
                        token[token_len] = '\0';
                        if ((p = strstr(token, "&nbsp")) != NULL){
                            *p = ' ';
                            token_len -= 4;
                            flag = 0;
                        }else if ((p = strstr(token, "&gt")) != NULL){
                            *p = '>';
                            token_len -= 2;
                            flag = 0;
                        }else if ((p = strstr(token, "&lt")) != NULL){
                            *p = '<';
                            token_len -= 2;
                            flag = 0;
                        }else if ((p = strstr(token, "&amp")) != NULL){
                            *p = '&';
                            token_len -= 3;
                            flag = 0;
                        }else if ((p = strstr(token, "&quot")) != NULL){
                            *p = '\"';
                            token_len -= 4;
                            flag = 0;
                        }else if ((p = strstr(token, "&apos")) != NULL){
                            *p = '\'';
                            token_len -= 4;
                            flag = 0;
                        }else if ((p = strstr(token, "&middot")) != NULL){
                            *p = '.';
                            token_len -= 6;
                            flag = 0;
                        }else if ((p = strstr(token, "&#229")) != NULL){
                            *p = '@';
                            token_len -= 4;
                            flag = 0;
                        }
                    }
					fscanf(pfile, "%c", &ch);
				}
				ungetc(ch, pfile);		//将获取的'<'回退回文档，以便下一次的获取
				token[token_len] = '\0';
				fprintf(pfile1, "inner: %s\n", token);
				sign = INNERTEXT;
				token_handle(token, token_len, sign);
				strcpy(token,"");
				token_len = 0;
			}
		}
	}
	head_token = p_token;
	fclose(pfile);
	fclose(pfile1);
	return TRUE;
}

void token_handle(char *token, int token_len, int sign){
    tail_token = (TOKEN *)malloc(sizeof(TOKEN));
    tail_token->elem = (char *)malloc(sizeof(char)*(token_len+1));
    strcpy(tail_token->elem, token);
    tail_token->type = sign;
    tail_token->next = head_token->next;
    head_token->next = tail_token;
    head_token = tail_token;
    return;
}

status Creat_HTML_Tree(Label_node **Root){
    TOKEN *token_temp = head_token->next;
    Label_node *Label_new;
    Label_node *stack[100];
    int top = -1;
    while(token_temp != NULL){
        if (token_temp->type == START || token_temp->type == NOEND){
            Label_new = (Label_node *)malloc(sizeof(Label_node));
            Label_new->classify = START;
            Label_new->info = NULL;
            Label_new->Label_value = NULL;
            Label_new->Para_label = NULL;
            Label_new->Parent_label = NULL;
            Label_new->Sub_label = NULL;
            strcpy(Label_new->label_name, token_temp->elem);
            if (top >= 0){
                if (stack[top]->Sub_label == NULL){
                    stack[top]->Sub_label = Label_new;
                    Label_new->Parent_label = stack[top];
                }else {
                    Label_node *p = stack[top]->Sub_label;
                    while(p->Para_label != NULL){
                        p = p->Para_label;
                    }
                    p->Para_label = Label_new;
                    Label_new->Parent_label = stack[top];
                }
            }else{
                *Root =Label_new;
            }
            if (token_temp->type == START)
                stack[++top] = Label_new;
        }else if (token_temp->type == END){
            top--;
        }else if (token_temp->type == INNERTEXT){
            stack[top]->info = token_temp->elem;
        }else if (token_temp->type == VALUE && stack[top] != NULL){
            Label_new = (Label_node *)malloc(sizeof(Label_node));
            Label_new->classify = VALUE;
            Label_new->info = token_temp->elem;
            Label_new->Label_value = NULL;
            Label_new->Para_label = NULL;
            Label_new->Parent_label = stack[top];
            Label_new->Sub_label = NULL;
            strcpy(Label_new->label_name, "href");
            stack[top]->Label_value = Label_new;
        }
        token_temp = token_temp->next;
    }
    //PreOrderTraverse(*Root);
    return 1;
}

status Free_HTML_Tree(Label_node **Root){
    //Free_HTML_List(head_token);
    Label_node *stack[100];
    int top = -1, sub = -1;
    stack[++top] = *Root;
    sub++;
    while(sub != top){
        if (stack[sub]->info != NULL){
            free(stack[sub]->info);
        }
        if (stack[sub]->Label_value != NULL){
            if (stack[sub]->Label_value->info != NULL)
                free(stack[sub]->Label_value->info);
            free(stack[sub]->Label_value);
        }
        if (stack[sub]->Para_label != NULL){
            stack[++top] = stack[sub]->Para_label;
        }
        if (stack[sub]->Sub_label != NULL){
            stack[++top] = stack[sub]->Sub_label;
        }
        free(stack[sub++]);
    }
    return;
}
/*
status Free_HTML_List(TOKEN **head_token){
    TOKEN *tail_token = head_token->next;
    while(tail_token != NULL){
        free(head_token);
        head_token = tail_token;
        tail_token = tail_token->next;
    }
    free(head_token);
    return 1;
}

void PreOrderTraverse(Label_node *T){
    if (T == NULL)
        return;
    if (T->info != NULL){
        printf("%s\n", T->info);
    }
    PreOrderTraverse(T->Sub_label);
    PreOrderTraverse(T->Para_label);
}
*/
int search_KeyWord(Label_node *T, char *Key, int *is_find){
    if (T == NULL)
        return 0;
    if (T->info != NULL){
        if (strstr(T->info, Key) != NULL){
            *is_find = 1;
            printf("%s", T->info);
            if (T->Label_value != NULL){
                printf(" %s", T->Label_value->info);
            }
            printf("\n\n");
        }
    }
    search_KeyWord(T->Para_label, Key, is_find);
    search_KeyWord(T->Sub_label, Key, is_find);
}

void search_Title(Label_node *T, char *Key, int *is_find){
    if (T == NULL)
        return;
    if (strcmp(T->label_name, "h1") == 0 || strcmp(T->label_name, "h2") == 0 || strcmp(T->label_name, "h3") == 0 ||
        strcmp(T->label_name, "h4") == 0 || strcmp(T->label_name, "h5") == 0 || strcmp(T->label_name, "h6") == 0 ||
        strcmp(T->label_name, "title") == 0){
            if (T->info != NULL && strstr(T->info, Key) != NULL){
                *is_find = 1;
                printf("%s ", T->info);
                if (T->Label_value != NULL){
                    printf("%s", T->Label_value->info);
                }
                printf("\n");
            }
        }
    search_Title(T->Para_label, Key, is_find);
    search_Title(T->Sub_label, Key, is_find);
}

void Traverse_Title(Label_node *T){
    if (T == NULL)
        return;
    if (strcmp(T->label_name, "h1") == 0 || strcmp(T->label_name, "h2") == 0 || strcmp(T->label_name, "h3") == 0 ||
        strcmp(T->label_name, "h4") == 0 || strcmp(T->label_name, "h5") == 0 || strcmp(T->label_name, "h6") == 0 ||
        strcmp(T->label_name, "title") == 0){
            if (T->info != NULL){
                printf("%s ", T->info);
                if (T->Label_value != NULL){
                    printf("%s", T->Label_value->info);
                }
                printf("\n");
            }
        }
    Traverse_Title(T->Para_label);
    Traverse_Title(T->Sub_label);
}
