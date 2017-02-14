#include"HTML_tree.h"

TOKEN *head_token = NULL, *tail_token = NULL;
Label_node *Root;

int main(void)
{
    Root = (Label_node*)malloc(sizeof(Label_node));
	int command;
	char buffer[MAX_LEN];
	printf("---------------------�˵�------------------\n");
	printf("            1:����HTML_DOM�ĵ���\n");
	printf("            2:���չؼ��ʲ����ı���Ϣ\n");
	printf("            3:�������±���\n");
	printf("            0:�˳�ϵͳ\n");
	printf("----------------��ӭʹ�ñ�ϵͳ-------------\n");
	while (scanf("%d", &command) == 1)
	{
		getchar();
		switch (command)
		{
		case 1:
			if (Creat_HTML_List() == TRUE && Creat_HTML_Tree(&Root) == TRUE)
                printf("HTML_DOM�ĵ����Ѵ����ɹ�\n");
			break;
		case 2:
			printf("��������Ҫ���ҵĹؼ���\n");
			scanf("%s", buffer);
			getchar();
			search_KeyWord(Root, buffer);
			break;
		case 3:
			printf("�������µı�������\n");
			search_Title(Root);
			break;
		case 0:
			printf("�˳�ϵͳ\n");
			return;
		default:
			break;
		}
		getchar();
		system("cls");
		printf("---------------------�˵�------------------\n");
        printf("            1:����HTML_DOM�ĵ���\n");
        printf("            2:���չؼ��ʲ����ı���Ϣ\n");
        printf("            3:�������±���\n");
        printf("            0:�˳�ϵͳ\n");
        printf("----------------��ӭʹ�ñ�ϵͳ-------------\n");
	}
	return 0;
}

status Creat_HTML_List(void)
{
	FILE *pfile = NULL;
	FILE *pfile1 = NULL;
	char ch;					//���������ȡ�ļ��е��ַ�
	char token[MAX_LEN] = "";   //������ʱ��Ŷ�ȡ������token
	char buffer[MAX_LEN] = "";  //������ʱ��Ŷ�ȡ�����ı�ǩ����
	int IS_ignore = FALSE;		//���ڱ��ע���Ƿ���ԣ�Ĭ��Ϊ���ɺ���
	int token_len = 0;			//���ڱ��token�ĳ���
    int flag_wbr = 0;			//���ڱ�������еı�ǩ�Ƿ����
	int sign = 0;
	int start = 0;
	int end = 0;

	head_token = (TOKEN *)malloc(sizeof(TOKEN));	//����Ԥ������ͷ�ڵ�
	head_token->next = NULL;
	TOKEN *p_token = head_token;		//���ڴ��Ԥ������ͷ�ڵ��ָ��

    char HTML_filename[100];   //���HTML������ĵ�
    printf("������Ҫ�򿪵��ĵ�:\n");
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

	while ((fscanf(pfile, "%c", &ch)) > 0)      //ֱ�������ı�����'\0'Ϊֹ
	{
		if(ch == '<')
		{
			fscanf(pfile, "%c", &ch);
			if(ch == '/')		//������ֱ����'/'��β��tokenֱ�Ӵ���
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
			else if(ch == '!')  //��ע�͵Ĵ���
            {
                fscanf(pfile, "%c", &ch);
                if(ch == '-')
                {
                    fscanf(pfile, "%c", &ch);   //ȡ��ע��!��--
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
            else                //�Կ�ʼ��ǩ�Ĵ���
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
                /*char *p = NULL;
                char *href[100];
                if ((p = strstr(token, "href")) != NULL){
                    sscanf(p, "%*[^\"]\"%[^\"]", href);
                    if (strstr(href, "www") != NULL){
                        printf("%s: %s\n", buffer, href);
                    }
                }*/
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
				ungetc(ch, pfile);		//����ȡ��'<'���˻��ĵ����Ա���һ�εĻ�ȡ
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
void token_handle(char *token, int token_len, int sign)
{
    tail_token = (TOKEN *)malloc(sizeof(TOKEN));
    tail_token->elem = (char *)malloc(sizeof(char)*(token_len+1));
    strcpy(tail_token->elem, token);
    tail_token->type = sign;
    tail_token->next = head_token->next;
    head_token->next = tail_token;
    head_token = tail_token;
    return;
}

status Creat_HTML_Tree(Label_node **Root)
{
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
        }
        token_temp = token_temp->next;
    }
    //PreOrderTraverse(*Root);
    return 1;
}
/*
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
int search_KeyWord(Label_node *T, char *Key){
    if (T == NULL)
        return 0;
    if (T->info != NULL){
        if (strstr(T->info, Key) != NULL){
            printf("  %s : %s\n", T->label_name, T->info);
            return 1;
        }
    }
    search_KeyWord(T->Para_label, Key);
    search_KeyWord(T->Sub_label, Key);
}

void search_Title(Label_node *T){
    if (T == NULL)
        return;
    if (strcmp(T->label_name, "h1") == 0 || strcmp(T->label_name, "h2") == 0 || strcmp(T->label_name, "h3") == 0 ||
        strcmp(T->label_name, "h4") == 0 || strcmp(T->label_name, "h5") == 0 || strcmp(T->label_name, "h6") == 0 ||
        strcmp(T->label_name, "title") == 0){
            if (T->info != NULL){
                printf("  %s : %s\n", T->label_name, T->info);
            }
            search_KeyWord(T->Sub_label, "");
        }
    search_Title(T->Para_label);
    search_Title(T->Sub_label);
}
