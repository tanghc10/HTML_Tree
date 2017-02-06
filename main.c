
#include"HTML_tree.h"

TOKEN *head_token = NULL, *tail_token = NULL;
ELEMTYPE *head_elemtype = NULL;

int main(void)
{
	Creat_HTML_List();
	test();
	return 0;
}

status Creat_HTML_List(void)
{
	FILE *pfile = NULL;
	char ch;					//������ʱ��Ż�ȡ���ַ�
	char token[MAX_LEN] = "";   //������ʱ��Ŷ�ȡ������token
	int IS_ignore = TRUE;		//���ڱ��ע���Ƿ���ԣ�Ĭ��Ϊ�ɺ���
	int IS_end = 0;				//���ڱ���Ƿ������
	int IS_attributes = 0;		//���ڱ���Ƿ�������
	int token_len = 0;			//���ڱ��token�ĳ���
	TOKEN *p_token = NULL;

	head_token = (TOKEN *)malloc(sizeof(TOKEN));
	head_token->next = NULL;
	p_token = head_token;

	if ((pfile = fopen(HTML_filename, "r")) == NULL)
	{
		printf("fail to open the file\n");
		return FALSE;
	}

	while ((fscanf(pfile, "%c", &ch)) > 0)      //ֱ�������ı������һ��'\0'Ϊֹ
	{
		if (ch == '<')
		{
			fscanf(pfile, "%c", &ch);
			if (ch == '/')		//������ֱ����'/'��β��tokenֱ�Ӵ���
			{
				IS_end = TRUE;
				fscanf(pfile, "%c", &ch);
				while (ch != ' ' && ch != '>' && ch != '/')
				{
					token[token_len++] = ch;
					fscanf(pfile, "%c", &ch);
				}
				while (ch != '>')
                {
                    fscanf(pfile, "%c", &ch);
                }
				token[token_len] = '\0';
				//strcpy(token,"");
				//token_len = 0;
				token_handle = end_handle;
				token_handle(token, &token_len, &IS_end);		//��token_len��IS_end�ָ���ʼ��
				continue;
			}
			else
			{
				IS_end = FALSE;
				while (ch != ' ' && ch != '>' && ch != '/' && ch != ':')
				{
					token[token_len++] = ch;
					fscanf(pfile, "%c", &ch);
				}
				while (ch != '>')
                {
                    fscanf(pfile, "%c", &ch);
                }
				token[token_len] = '\0';
				token_handle = start_handle;
				token_handle(token, &token_len, &IS_ignore);	//��token_len�ָ���ʼ�������ж��Ƿ���Ҫ���к��Ժ�����ı�
				if(strcmp(token, "style") ==0 || strcmp(token, "script") == 0 || strcmp(token, "noscript") == 0 || strcmp(token, "textarea") == 0)
                {
                    while(1){
                        fscanf(pfile, "%c", &ch);
                        if (ch == '<'){
                            fscanf(pfile, "%c", &ch);
                            if (ch == '/'){
                                fscanf(pfile, "%c", &ch);
                                int token_num = 0;
                                while(ch == token[token_num++]){
                                    fscanf(pfile, "%c", &ch);
                                }
                                if (ch == '>' && token[token_num] == '\0');
                                    break;
                            }
                        }
                    }
                }
                strcpy(token,"");
				token_len = 0;
				continue;
			}
		}
		else if(ch == '\n' || ch == ' ')
            continue;
		else
		{
			if (!IS_ignore)
			{
				while (ch != '<')
				{
					token[token_len++] = ch;
					fscanf(pfile, "%c", &ch);
				}
				ungetc(ch, pfile);		//����ȡ��'<'���˻��ĵ����Ա���һ�εĻ�ȡ
				token[token_len] = '\0';
				//printf("innertext:%s\n",token);
				//strcpy(token,"");
				//token_len = 0;
				//IS_ignore = TRUE;
				token_handle = innertext_handle;
				token_handle(token, &token_len, &IS_ignore);	//��IS_ignore�ĳ�Ĭ�ϵĿ��Ժ���
			}
			else
				continue;
		}
	}
	head_token = p_token;
	fclose(pfile);
	return TRUE;
}
void start_handle(char *token, int *token_len, int *sign)
{
    char *pcPos = NULL;
    char buffer[200];		//�����ݴ��ǩ����������
    int len = 0;
    int flag_lable = 1;
	if (*token == '!')	//�����ע��<!--****-->
	{
		*token = 0;
		return ;
	}
	else
	{
		if ((pcPos = strchr(token, ' ')) == NULL && (pcPos = strchr(token, '/') == NULL))	//����������ԣ��Ҳ�����'/'��β��token
		{
			tail_token = (TOKEN *)malloc(sizeof(TOKEN));
			tail_token->elem = (char *)malloc(sizeof(char)*(*token_len + 1));
			strcpy(tail_token->elem, token);
			if (strcmp(token, "br") == 0)
			{
				tail_token->type = NOEND;
				*sign = FALSE;
			}
			else if (strcmp(token, "wbr") == 0)
			{
				if (flag_wbr)
				{
					tail_token->type = END;
					flag_wbr--;
				}
				else
				{
					tail_token->type = START;
					flag_wbr++;
				}
				*sign = FALSE;
			}
			else if (strstr(token, "style") != NULL || strstr(token, "script") != NULL || strstr(token, "noscript") != NULL || strstr(token, "textarea") != NULL)
			{
				tail_token->type = START;
				*sign = TRUE;
			}
			else
            {
                tail_token->type = START;
                *sign = FALSE;
            }
            tail_token->next = head_token->next;
            head_token->next = tail_token;
            head_token = tail_token;
		}
		else
		{
			if (*(token + *token_len -1) == '/')	//���Ϊ<   />�ĵ�����ǩ
			{
				pcPos = token;
				while (*pcPos != '/')
				{
					strcpy(buffer, "");
					len = 0;
					while (*pcPos != ' ' || !pcPos)
					{
						buffer[len++] = *pcPos;
						pcPos++;
					}
					if(!len)
                        break;
					buffer[len] = '\0';
					tail_token = (TOKEN *)malloc(sizeof(TOKEN));
                    tail_token->elem = (char *)malloc(sizeof(char)*(len + 1));
                    strcpy(tail_token->elem, buffer);
                    tail_token->next = head_token->next;
                    head_token->next = tail_token;
                    head_token = tail_token;
					if (flag_lable)	//��Ϊ��ǩ��ʱ��
					{
						tail_token->type = NOEND;
						flag_lable = 0;
					}
					else
					{
						tail_token->type = ATTRIBUTES;
					}
					pcPos++;
				}
			}
			else
			{
			    pcPos = token;
				while (*pcPos != '\0')
				{
					strcpy(buffer, "");
					len = 0;
					while (*pcPos != ' ' && *pcPos != '\0')
					{
						buffer[len++] = *pcPos;
						pcPos++;
					}
					if(!len)
                        break;
					buffer[len] = '\0';
					tail_token = (TOKEN *)malloc(sizeof(TOKEN));
                    tail_token->elem = (char *)malloc(sizeof(char)*(len + 1));
                    strcpy(tail_token->elem, buffer);
                    tail_token->next = head_token->next;
                    head_token->next = tail_token;
                    head_token = tail_token;
					if (flag_lable)	//��Ϊ��ǩ��ʱ��
					{
					    if(strcmp(buffer,"meta") == 0 || strcmp(buffer, "track") == 0 || strcmp(buffer, "source") == 0)
                            tail_token->type = NOEND;
                        else
                            tail_token->type = START;
						flag_lable = 0;
					}
					else
					{
						tail_token->type = ATTRIBUTES;
					}
					pcPos++;
				}
			}
		}
	}
}
void end_handle(char *token, int *token_len, int *sign)
{
	tail_token = (TOKEN *)malloc(sizeof(TOKEN));
	tail_token->elem = (char *)malloc(sizeof(char)*(*token_len + 1));
	if(!tail_token || !tail_token->elem)
    {
        printf("�ڴ����ʧ��\n");
        exit(FALSE);
    }
	strcpy(tail_token->elem, token);
	tail_token->type = END;
	tail_token->next = head_token->next;
	head_token->next = tail_token;
	head_token = tail_token;
	strcpy(token,"");
	*token_len = 0;
	*sign = FALSE;
	return ;
}
void innertext_handle(char *token, int *token_len, int *sign)
{
	tail_token = (TOKEN *)malloc(sizeof(TOKEN));
	tail_token->elem = (char *)malloc(sizeof(char)*(*token_len + 1));
	if(!tail_token || !tail_token->elem)
    {
        printf("�ڴ����ʧ��\n");
        exit(FALSE);
    }
	strcpy(tail_token->elem, token);
	tail_token->type = INNERTEXT;
	tail_token->next = head_token->next;
	head_token->next = tail_token;
	head_token = tail_token;
	strcpy(token,"");
	*token_len = 0;
	*sign = TRUE;
	return ;
}
void test(void)
{
    FILE *pfile;
    tail_token = head_token->next;

    if((pfile = fopen("output.txt", "w")) == NULL)
        printf("�ļ���ʧ��\n");
    while(tail_token != NULL)
    {
        fprintf(pfile, "%s\n",tail_token->elem);
        tail_token=tail_token->next;
    }
    fclose(pfile);
}
